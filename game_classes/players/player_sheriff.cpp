#include "player_sheriff.h"

#include <vector>
#include <ranges>
#include <random>
#include <algorithm>


namespace NMafia {

    bool getRandomBool() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);
        return dis(gen) == 1;
    }

    PlayerAction TPlayerSheriff::NigthAction() {
        if (getRandomBool()) {
            Kill();
        } else {
            Check();
        }
        co_return;
    }

    void TPlayerSheriff::Kill() {
        auto target = ChooseTargetToKill();
        TLogger::multiLog(LogPaths,
            "Sheriff wants to kill " + target->GetId()
        );
        WriteMsgByRole(
            {
                {"message", "Sheriff wants to kill"},
                {"id", target->GetId()},
            },
            ERoles::Leader
        );
    }

    void TPlayerSheriff::Check() {
        auto target = ChooseTargetToCheck();
        if (!target) {
            Kill();
            return;
        }
        auto targetRoles = target->GetRoles();
        CheckedIds.insert(target->GetId());
        TLogger::multiLog(LogPaths,
            "Sheriff " + GetId() + " checks " + target->GetId()
        );
        if (targetRoles.find(ERoles::Mafia) != targetRoles.end()) {
            TrustTable[target->GetId()] -= 50;
        } else {
            TrustTable[target->GetId()] += 50;
        }
        TLogger::multiLog(LogPaths,
            "Sheriff " + GetId() + " now trust to " + target->GetId() + " like " + std::to_string(TrustTable[target->GetId()])
        );
    }

    TSharedPtr<TPlayerBase> TPlayerSheriff::ChooseTargetToKill()
    {
        std::vector<Id> ids;
        std::ranges::copy(*IdToPlayerPtr | std::views::keys, std::back_inserter(ids));

        int minTrust = std::ranges::min(
            ids | std::views::transform([this](const Id& id) {
                return TrustTable[id];
            })
        );

        std::vector<Id> suspiciousPlayers;
        std::ranges::copy(
            ids | std::views::filter([this, minTrust](const Id& id) {
                return (TrustTable[id] == minTrust)
                    && (id != GetId())
                    && (!IsLeader(id))
                    && (IsInGame(id))
                    && (IsAlive(id));
            }),
            std::back_inserter(suspiciousPlayers)
        );

        if (suspiciousPlayers.size() >= 1) {
            std::random_device rd;
            std::mt19937 gen(rd());
            Id choosenId;
            choosenId = suspiciousPlayers[std::uniform_int_distribution<>(0, suspiciousPlayers.size() - 1)(gen)];
            return IdToPlayerPtr->at(choosenId);
        } else {
            return IdToPlayerPtr->at(suspiciousPlayers[0]);
        }
    }

    TSharedPtr<TPlayerBase> TPlayerSheriff::ChooseTargetToCheck() {
        std::vector<Id> ids;
        std::ranges::copy(*IdToPlayerPtr | std::views::keys, std::back_inserter(ids));

        auto range = ids | std::views::transform([this](const Id& id) {
            return TrustTable[id];
        });
        int minTrust = std::ranges::min(range);
        int maxTrust = std::ranges::max(range);

        int upThreshold = maxTrust * 0.85;
        int downThreshold = minTrust * 0.85;

        std::vector<Id> suspiciousPlayers;
        std::ranges::copy(
            ids | std::views::filter([&, this](const Id& id) {
                return (TrustTable[id] <= upThreshold)
                    && (TrustTable[id] >= downThreshold)
                    && (IdToPlayerPtr->at(id)->GetStatus() != EStatus::Dead)
                    && (CheckedIds.find(id) == CheckedIds.end())
                    && (id != GetId())
                    && (!IsLeader(id))
                    && (IsInGame(id));
            }),
            std::back_inserter(suspiciousPlayers)
        );

        std::random_device rd;
        std::mt19937 gen(rd());
        Id choosenId;
        if (suspiciousPlayers.empty()) {;
            return TSharedPtr<TPlayerBase>();
        } else {
            choosenId = suspiciousPlayers[std::uniform_int_distribution<>(0, suspiciousPlayers.size() - 1)(gen)];
            return IdToPlayerPtr->at(choosenId);
        }
    }
}
