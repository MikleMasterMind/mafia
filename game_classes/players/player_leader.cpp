#include "player_leader.h"

#include <ranges>
#include <future>


#define DO_ALL_PLAYERS_ACTIONS(action) \
    std::vector<std::future<void>> futures; \
    for (const auto& id : *IdToPlayerPtr | std::views::keys \
        | std::views::filter([&](const Id& id) { \
            return (!IsLeader(id)) \
                && (IsInGame(id)) \
                && (IsAlive(id)); \
    })) { \
        futures.push_back(std::async(std::launch::async, [&]() { \
            auto action = IdToPlayerPtr->at(id)->action(); \
            action.handle.resume(); \
        })); \
    } \
    \
    for (auto& future : futures) { \
        future.get(); \
    }

#define DO_PROCESS_KILL(id, killer) \
    if (id != "") { \
        auto target = IdToPlayerPtr->at(id); \
        if (target->GetStatus() != EStatus::Protected) { \
            std::string killerStr = killer; \
            WriteMsgByRole( \
                { \
                    {"announc", killerStr + " killed " + target->GetId()}, \
                }, \
                ERoles::Default \
            ); \
            target->SetStatus(EStatus::Excluded); \
            TLogger::Log( \
                killerStr + " killed " + id \
            ); \
        } \
    } \
    id = "";



namespace NMafia {
    auto GetChoosenTarget = [](std::unordered_map<Id, int>& table) -> Id {
        int maxVoite = std::ranges::max(table | std::views::values);
        auto choosen =  std::find_if(table.begin(), table.end(), [&](const auto& pair) {
            return maxVoite == pair.second; \
        })->first;
        for (auto& voites : table | std::views::values) {
            voites = 0;
        }
        return choosen;
    };

    PlayerAction TPlayerLeader::DayAction() {
        DO_ALL_PLAYERS_ACTIONS(DayAction);
        ProcessDayVoiting();
        co_return;
    }

    PlayerAction TPlayerLeader::NigthAction() {
        DO_ALL_PLAYERS_ACTIONS(NigthAction);
        ProcessMafiaVoiting();
        ProcessSheriffKill();
        ProcessManiacKill();
        CleanDoctorHealing();
        co_return;
    }

    void TPlayerLeader::ProcessSingleMessage(const TMessage &msg) {
        // TLogger::Log(
        //     "Player " + GetId() + " got message " + msg.Body.ToString() + " from " + msg.Src
        // );
        if (msg.Body.GetOrNull("message") == "Voite again") {
            ++DayVoiteTable[msg.Body.Get("id")];
        } else if (msg.Body.GetOrNull("message") == "Mafia voite again") {
            ++MafiaVoiteTable[msg.Body.Get("id")];
        } else if (msg.Body.GetOrNull("message") == "Sheriff wants to kill") {
            TargetToKillForSheriff = msg.Body.Get("id");
        } else if (msg.Body.GetOrNull("message") == "Maniac wants to kill") {
            TargetToKillForManiac = msg.Body.Get("id");
        }
    }

    void TPlayerLeader::ProcessDayVoiting() {
        auto choosenTarget = GetChoosenTarget(DayVoiteTable);
        TLogger::Log(
            "Player " + choosenTarget + " choosen on day voiting"
        );
        WriteMsgByRole(
            {
                {"announc", "Player " + choosenTarget + " choosen on day voiting"},
            },
            ERoles::Default
        );
        IdToPlayerPtr->at(choosenTarget)->SetStatus(EStatus::Excluded);
    }

    void TPlayerLeader::ProcessMafiaVoiting()  {
        auto choosenTarget = GetChoosenTarget(MafiaVoiteTable);
        TLogger::Log(
            "Player " + choosenTarget + " choosen on mafia voiting"
        );
        WriteMsgByRole(
            {
                {"announc", "Player " + choosenTarget + " choosen on mafia voiting"},
            },
            ERoles::Mafia
        );
        if (IdToPlayerPtr->at(choosenTarget)->GetStatus() != EStatus::Protected) {
            WriteMsgByRole(
                {
                    {"announc", "Mafia killed " + choosenTarget},
                },
                ERoles::Default
            );
            IdToPlayerPtr->at(choosenTarget)->SetStatus(EStatus::Excluded);
        }
    }

    void TPlayerLeader::ProcessSheriffKill() {
        DO_PROCESS_KILL(TargetToKillForSheriff, "Sheriff");
    }

    void TPlayerLeader::ProcessManiacKill() {
        DO_PROCESS_KILL(TargetToKillForManiac, "Maniac");
    }

    void TPlayerLeader::CleanDoctorHealing() {
        for (auto& player : *IdToPlayerPtr | std::views::values | std::views::filter([&](const auto& player) {
            return player->GetStatus() == EStatus::Protected;
        })) {
            player->SetStatus(EStatus::Alive);
        }
    }

    bool TPlayerLeader::GameEnded() {
        std::vector<TSharedPtr<TPlayerBase>> players;
        std::ranges::copy(*IdToPlayerPtr | std::views::values, std::back_inserter(players));

        int peacefullCount = std::count_if(players.begin(), players.end(), [this](const TSharedPtr<TPlayerBase>& player) {
            auto roles = player->GetRoles();
            return (IsAlive(player->GetId()))
                && (IsInGame(player->GetId()))
                && (roles.find(ERoles::Peacefull) != roles.end())
                && (roles.find(ERoles::Maniac) == roles.end());
        });
        int mafiaCount = std::count_if(players.begin(), players.end(), [this](const TSharedPtr<TPlayerBase>& player) {
            auto roles = player->GetRoles();
            return (IsAlive(player->GetId()))
                && (IsInGame(player->GetId()))
                && (roles.find(ERoles::Mafia) != roles.end());
        });
        int maniacCount = std::count_if(players.begin(), players.end(), [this](const TSharedPtr<TPlayerBase>& player) {
            auto roles = player->GetRoles();
            return (IsAlive(player->GetId()))
                && (IsInGame(player->GetId()))
                && (roles.find(ERoles::Maniac) != roles.end());
        });

        if (peacefullCount + mafiaCount + maniacCount == 0) {
            GameResult = "All dead";
            return true;
        } else if (mafiaCount + maniacCount == 0) {
            GameResult = "Peacefull win!!!";
            return true;
        } else if (maniacCount == 1 && peacefullCount <= 1 && mafiaCount == 0) {
            GameResult = "Maniac win!!!";
            return true;
        } else if ((mafiaCount > maniacCount + peacefullCount) || (mafiaCount >= peacefullCount && maniacCount == 0)) {
            GameResult = "Mafia win!!!";
            return true;
        } else {
            return false;
        }
    }

    void TPlayerLeader::SayResult() {
        TLogger::Log(GameResult);
        WriteMsgByRole(
            {
                {"announc", GameResult}
            },
            ERoles::Default
        );
    }
}

#undef DO_ALL_PLAYERS_ACTIONS
#undef DO_PROCESS_KILL
