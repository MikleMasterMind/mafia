#include "player_sheriff.h"

#include <vector>
#include <ranges>
#include <random>
#include <algorithm>
#include <regex>

namespace NMafia {

    bool getRandomBool() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);
        return dis(gen) == 1;
    }

    PlayerAction TPlayerSheriff::NigthAction() {
        if (getRandomBool()) {
            auto target = ChooseTargetToKill();
            if (target->GetStatus() == EStatus::Alive) {
                target->SetStatus(EStatus::Dead);
            }
        } else {
            auto target = ChooseTargetToCheck();
            auto targetRoles = target->GetRoles();
            if (targetRoles.find(ERoles::Mafia) != targetRoles.end()) {
                TrustTable[target->GetId()] -= 50;
            } else {
                TrustTable[target->GetId()] += 50;
            }
        }
        co_return;
    }

    TSharedPtr<TPlayerPlayable> TPlayerSheriff::ChooseTargetToKill() {
        std::vector<Id> ids;
        std::ranges::copy(*IdToPlayerPlayablePtr | std::views::keys, std::back_inserter(ids));

        int minTrust = std::ranges::min(
            ids | std::views::transform([this](const Id& id) {
                return TrustTable[id];
            })
        );

        std::vector<Id> suspiciousPlayers;
        std::ranges::copy(
            ids | std::views::filter([this, minTrust](const Id& id) {
                return TrustTable[id] == minTrust;
            }),
            std::back_inserter(suspiciousPlayers)
        );

        if (suspiciousPlayers.size() >= 1) {
            std::random_device rd;
            std::mt19937 gen(rd());
            Id choosenId;
            choosenId = suspiciousPlayers[std::uniform_int_distribution<>(0, suspiciousPlayers.size() - 1)(gen)];
            return IdToPlayerPlayablePtr->at(choosenId);
        } else {
            return IdToPlayerPlayablePtr->at(suspiciousPlayers[0]);
        }
    }

    TSharedPtr<TPlayerPlayable> TPlayerSheriff::ChooseTargetToCheck() {
        std::vector<Id> ids;
        std::ranges::copy(*IdToPlayerPlayablePtr | std::views::keys, std::back_inserter(ids));

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
                return (TrustTable[id] <= upThreshold) && (TrustTable[id] >= downThreshold);
            }),
            std::back_inserter(suspiciousPlayers)
        );

        std::random_device rd;
        std::mt19937 gen(rd());
        Id choosenId;
        if (suspiciousPlayers.empty()) {;
            choosenId = ids[std::uniform_int_distribution<>(0, ids.size() - 1)(gen)];
        } else {
            choosenId = suspiciousPlayers[std::uniform_int_distribution<>(0, suspiciousPlayers.size() - 1)(gen)];
        }

        return IdToPlayerPlayablePtr->at(choosenId);
    }
}
