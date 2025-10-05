#include "player_playable.h"

#include <vector>
#include <ranges>
#include <random>
#include <algorithm>
#include "../../library/logger/logger.h"


namespace NMafia {
    PlayerAction NMafia::TPlayerPlayable::DayAction() {
        Voite();
        co_return;
    }

    void TPlayerPlayable::Voite() {
        auto target = ChooseTargretToVoite();
        WriteMsgByRole(
            {
                {"message", "Voite again "},
                {"id", target->GetId()},
            },
            ERoles::Default
        );
    }

    TSharedPtr<TPlayerBase> TPlayerPlayable::ChooseTargretToVoite() {
        std::vector<Id> ids;
        std::ranges::copy(*IdToPlayerPtr | std::views::keys, std::back_inserter(ids));

        int minTrust = std::ranges::min(
            ids | std::views::transform([this](const Id& id) {
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
        if (msg.Body.Contains("Voite again")) {
            Id extractedId = msg.Body.Get("id");
            if (extractedId == GetId()) {
                TrustTable[msg.Src] -= 5;
            }
        }
    }
}
