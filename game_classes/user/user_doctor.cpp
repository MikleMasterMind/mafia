#include "user_doctor.h"

#include <iostream>
#include <ranges>


namespace NMafia {
    TSharedPtr<TPlayerBase> TUserDoctor::ChooseTargetToHeal() {
        std::vector<Id> ids;
        std::ranges::copy(*IdToPlayerPtr | std::views::keys | std::views::filter([this](const Id& id) {
                return (!IsLeader(id)
                    && (IsInGame(id))
                    && (id != GetId()));
            }),
            std::back_inserter(ids)
        );

        std::cout << "Choose player to heal:" << std::endl;
        for (size_t i = 0; i < ids.size(); ++i) {
            std::cout << "Player " + ids[i] << std::endl;
        }

        Id choosenId;
        do {
            std::cout << "Choose id from list: ";
            std::cin >> choosenId;
        } while (std::count_if(ids.begin(), ids.end(), [&](const Id& id) { return id == choosenId; }) == 0);

        return IdToPlayerPtr->at(choosenId);
    }
}
