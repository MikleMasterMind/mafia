#include "player_leader.h"

#include <ranges>
#include <future>
#include <iostream>


#define DO_ALL_PLAYERS_ACTIONS(action) \
    std::vector<std::future<void>> futures; \
    for (const auto& id : *IdToPlayerPtr | std::views::keys \
        | std::views::filter([&](const Id& id) { \
            return (!IsLeader(id)) \
                && (IsInGame(id)); \
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
            CountPlayers(); \
            TLogger::Log( \
                killerStr + " killed " + id \
            ); \
        } \
    } \
    id = "";



namespace NMafia {
    auto GetChoosenTarget = [](std::unordered_map<Id, int>& table) -> Id {
        if (table.size() == 0) {
            return "";
        }
        auto maxVote = std::ranges::max(table | std::views::values);
        auto it = std::ranges::find_if(table, [maxVote](const auto& pair) {
            return pair.second == maxVote;
        });
        Id choosen = (it != table.end()) ? it->first : "";
        for (auto& vote : table | std::views::values) {
            vote = 0;
        }
        return choosen;
    };

    PlayerAction TPlayerLeader::DayAction() {
        DO_ALL_PLAYERS_ACTIONS(DayAction);
        ProcessDayVoting();
        CleanDoctorHealing();
        co_return;
    }

    PlayerAction TPlayerLeader::NigthAction() {
        DO_ALL_PLAYERS_ACTIONS(NigthAction);
        ProcessMafiaVoting();
        ProcessSheriffKill();
        ProcessManiacKill();
        CleanDoctorHealing();
        co_return;
    }

    void TPlayerLeader::ProcessSingleMessage(const TMessage &msg) {
        if (msg.Body.GetOrNull("message") == "Vote again") {
            auto id = msg.Body.Get("id");
            if (DayVoteTable.find(id) != DayVoteTable.end()) {
                ++DayVoteTable[id];
            } else {
                DayVoteTable[id] = 1;
            }
        } else if (msg.Body.GetOrNull("message") == "Mafia vote again") {
            auto id = msg.Body.Get("id");
            if (MafiaVoteTable.find(id) != MafiaVoteTable.end()) {
                ++MafiaVoteTable[id];
            } else {
                MafiaVoteTable[id] = 1;
            }
        } else if (msg.Body.GetOrNull("message") == "Sheriff wants to kill") {
            TargetToKillForSheriff = msg.Body.Get("id");
        } else if (msg.Body.GetOrNull("message") == "Maniac wants to kill") {
            TargetToKillForManiac = msg.Body.Get("id");
        }
    }

    void TPlayerLeader::ProcessDayVoting() {
        auto choosenTarget = GetChoosenTarget(DayVoteTable);
        if (choosenTarget == "") {
            TLogger::Log(
                "Nobody choosen on day voting"
            );
            WriteMsgByRole(
                {
                    {"announc", "Nobody choosen on day voting"},
                },
                ERoles::Default
            );
        } else {
            TLogger::Log(
                "Player " + choosenTarget + " choosen on day voting"
            );
            WriteMsgByRole(
                {
                    {"announc", "Player " + choosenTarget + " choosen on day voting"},
                },
                ERoles::Default
            );
            IdToPlayerPtr->at(choosenTarget)->SetStatus(EStatus::Excluded);
            CountPlayers();
        }
    }

    void TPlayerLeader::ProcessMafiaVoting()  {
        auto choosenTarget = GetChoosenTarget(MafiaVoteTable);
        if (choosenTarget == "") {
            TLogger::Log(
                "Nobody choosen on mafia voting"
            );
            WriteMsgByRole(
                {
                    {"announc", "Nobody choosen on mafia voting"},
                },
                ERoles::Default
            );
        } else {
            TLogger::Log(
                "Player " + choosenTarget + " choosen on mafia voting"
            );
            WriteMsgByRole(
                {
                    {"announc", "Player " + choosenTarget + " choosen on mafia voting"},
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
                CountPlayers();
            }
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

    void TPlayerLeader::CountPlayers() {
        std::vector<TSharedPtr<TPlayerBase>> players;
        std::ranges::copy(*IdToPlayerPtr | std::views::values, std::back_inserter(players));

        auto countRoles = [&, this](ERoles role) -> int {
            return std::count_if(players.begin(), players.end(),
                [&, this](const TSharedPtr<TPlayerBase>& player) {
                    auto roles = player->GetRoles();
                    return (IsInGame(player->GetId()))
                        && (roles.find(role) != roles.end());
                }
            );
        };

        PeacefullCount = countRoles(ERoles::Peacefull);
        MafiaCount = countRoles(ERoles::Mafia);
        ManiacCount = countRoles(ERoles::Maniac);
    }

    bool TPlayerLeader::GameEnded() {
        if (PeacefullCount + MafiaCount + ManiacCount == 0) {
            GameResult = "All dead";
            return true;
        } else if (MafiaCount + ManiacCount == 0) {
            GameResult = "Peacefull win!!!";
            return true;
        } else if (ManiacCount == 1 && PeacefullCount <= 1 && MafiaCount == 0) {
            GameResult = "Maniac win!!!";
            return true;
        } else if ((MafiaCount > ManiacCount + PeacefullCount) || (MafiaCount >= PeacefullCount && ManiacCount == 0)) {
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
