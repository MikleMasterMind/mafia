#include "user_maniac.h"

#include <iostream>
#include <ranges>

namespace NMafia {
    TSharedPtr<TPlayerBase> TUserManiac::ChooseTargetToKill() {
        std::vector<Id> ids;
        std::ranges::copy(*IdToPlayerPtr | std::views::keys | std::views::filter([this](const auto& id) {
                return (id != GetId())
                    && (!IsLeader(id))
                    && (IsAlive(id));
            }),
            std::back_inserter(ids)
        );

        std::cout << "Choose player to kill:" << std::endl;
        for (size_t i = 0; i < ids.size(); ++i) {
            std::cout << "Player " + ids[i] << std::endl;
        }

        Id choosenId;
        do {
            std::cout << "Choose id from list: " << std::endl;
            std::cin >> choosenId;
        } while (std::count_if(ids.begin(), ids.end(), [&](const Id& id) { return id == choosenId; }) == 0);

        return IdToPlayerPtr->at(choosenId);
    }
}
