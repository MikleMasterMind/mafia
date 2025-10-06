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
            if (IdToPlayerPtr->at(id)->GetStatus() != EStatus::Dead) { \
                auto action = IdToPlayerPtr->at(id)->action(); \
                action.handle.resume(); \
            } \
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
            std::string message = killer; \
            message += " killed " + id; \
            WriteMsgByRole( \
                { \
                    {"message", message}, \
                }, \
                ERoles::Default \
            ); \
            target->SetStatus(EStatus::Excluded); \
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
        std::cout << "begin day action\n";
        DO_ALL_PLAYERS_ACTIONS(DayAction);
        std::cout << "players did their day actions\n";
        ProcessDayVoiting();
        std::cout << "leader processed day voiting\n";
        CheckGameEnded();
        std::cout << "end day action\n";
        co_return;
    }

    PlayerAction TPlayerLeader::NigthAction() {
        std::cout << "begin nigth action\n";
        DO_ALL_PLAYERS_ACTIONS(NigthAction);
        std::cout << "players did their nigth actions\n";
        ProcessMafiaVoiting();
        std::cout << "leader processed mafia voiting\n";
        ProcessSheriffKill();
        std::cout << "leader processed sheriff killing\n";
        ProcessManiacKill();
        std::cout << "leader processed maniac killing\n";
        CleanDoctorHealing();
        std::cout << "leader cleaned doctor healing\n";
        CheckGameEnded();
        std::cout << "end nigth aciton\n";
        co_return;
    }

    void TPlayerLeader::ProcessSingleMessage(const TMessage &msg) {
        // TLogger::multiLog(LogPaths,
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
        TLogger::multiLog(LogPaths,
            "Player " + choosenTarget + " choosen on day voiting"
        );
        WriteMsgByRole(
            {
                {"message", "Player " + choosenTarget + " choosen on day voiting"},
            },
            ERoles::Default
        );
        IdToPlayerPtr->at(choosenTarget)->SetStatus(EStatus::Excluded);
    }

    void TPlayerLeader::ProcessMafiaVoiting()  {
        auto choosenTarget = GetChoosenTarget(MafiaVoiteTable);
        TLogger::multiLog(LogPaths,
            "Player " + choosenTarget + " choosen on mafia voiting"
        );
        WriteMsgByRole(
            {
                {"message", "Player " + choosenTarget + " choosen on mafia voiting"},
            },
            ERoles::Mafia
        );
        if (IdToPlayerPtr->at(choosenTarget)->GetStatus() != EStatus::Protected) {
            WriteMsgByRole(
                {
                    {"message", "Mafia killed " + choosenTarget},
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

    void TPlayerLeader::CheckGameEnded() {
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

        if (mafiaCount == 0 && maniacCount == 0) { // peacefull win
            *GameEnded = true;
            TLogger::multiLog(LogPaths,
                "Peacefull win!!!"
            );
        } else if (maniacCount == 1 && peacefullCount == 1 && mafiaCount == 0) { // maniac win
            *GameEnded = true;
            TLogger::multiLog(LogPaths,
                "Maniac win!!!"
            );
        } else if ((mafiaCount > peacefullCount) || (mafiaCount >= peacefullCount && maniacCount == 0)) { // mafia win
            *GameEnded = true;
            TLogger::multiLog(LogPaths,
                "Mafia win!!!"
            );
        }
    }
}

#undef DO_ALL_PLAYERS_ACTIONS
#undef DO_PROCESS_KILL
