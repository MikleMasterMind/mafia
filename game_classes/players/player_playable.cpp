#include "player_playable.h"

#include <vector>
#include <ranges>
#include <random>
#include <algorithm>


namespace NMafia {
    PlayerAction NMafia::TPlayerPlayable::DayAction() {
        Vote();
        co_return;
    }

    void TPlayerPlayable::Vote() {
        auto target = ChooseTargretToVote();
        WriteMsgByRole(
            {
                {"message", "Vote again"},
                {"id", target->GetId()},
            },
            ERoles::Default
        );
        TLogger::Log(
            "Player " + GetId() + " vote again " + target->GetId()
        );
    }

    TSharedPtr<TPlayerBase> TPlayerPlayable::ChooseTargretToVote() {
        std::vector<Id> ids;
        std::ranges::copy(*IdToPlayerPtr | std::views::keys | std::views::filter([this](const Id& id) {
                return (id != GetId()
                    && (!IsLeader(id))
                    && (IsInGame(id)));
            }),
            std::back_inserter(ids)
        );

        int minTrust = std::ranges::min(
            ids | std::views::transform([this](const Id& id) {
                std::lock_guard lock(TrustTableMutex);
                if (TrustTable.find(id) == TrustTable.end()) {
                    return 0;
                }
                return TrustTable[id];
            })
        );
        int threshold = minTrust * 0.85;

        std::vector<Id> suspiciousPlayers;
        std::ranges::copy(
            ids | std::views::filter([this, threshold](const Id& id) {
                return TrustTable[id] <= threshold;
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

        return IdToPlayerPtr->at(choosenId);
    }

    void TPlayerPlayable::ProcessSingleMessage(const TMessage &msg) {
        // TLogger::Log(
        //     "Player " + GetId() + " got message " + msg.Body.ToString() + " from " + msg.Src
        // );
        if (msg.Body.GetOrNull("message") == "Vote again") {
            Id extractedId = msg.Body.Get("id");
            if (extractedId == GetId()) {
                std::lock_guard lock(TrustTableMutex);
                if (TrustTable.find(msg.Src) == TrustTable.end()) {
                    TrustTable[msg.Src] = 0;
                }
                TrustTable[msg.Src] -= 5;
                TLogger::Log(
                    "Player " + GetId() + " now trust to " + msg.Src + " like " + std::to_string(TrustTable[msg.Src])
                );
            }
        }
    }
}
