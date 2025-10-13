#include "player_maniac.h"

#include <vector>
#include <ranges>
#include <random>
#include <algorithm>


namespace NMafia {
    PlayerAction TPlayerManiac::NigthAction() {
        auto target = ChooseTargetToKill();
        TLogger::Log(
            "Maniac wants to kill " + target->GetId()
        );
        WriteMsgByRole(
            {
                {"message", "Maniac wants to kill"},
                {"id", target->GetId()},
            },
            ERoles::Leader
        );
        co_return;
    }

    TSharedPtr<TPlayerBase> TPlayerManiac::ChooseTargetToKill() {
        std::vector<Id> ids;
        std::ranges::copy(*IdToPlayerPtr | std::views::keys | std::views::filter([this](const Id& id) {
                return (id != GetId())
                    && (!IsLeader(id))
                    && (IsInGame(id));
            }),
            std::back_inserter(ids)
        );

        std::random_device rd;
        std::mt19937 gen(rd());
        Id choosenId = ids[std::uniform_int_distribution<>(0, ids.size() - 1)(gen)];

        return IdToPlayerPtr->at(choosenId);
    }
}
