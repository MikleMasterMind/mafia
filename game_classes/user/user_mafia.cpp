#include "user_mafia.h"

#include <iostream>
#include <ranges>

namespace NMafia {
    PlayerAction TUserMafia::NigthAction() {
        if (FirstNigth) {
            FirstNigth = false;
            WriteToView("Mafia players:");
            for (const auto& id : *IdToPlayerPtr | std::views::keys | std::views::filter([this](const Id& id) {
                return IsMafia(id);
            })) {
                WriteToView("Player: " + id);
            }
        }
        return TPlayerMafia::NigthAction();
    }

    TSharedPtr<TPlayerBase> TUserMafia::ChooseTargetToMafiaVoite() {
        std::vector<Id> ids;
        std::ranges::copy(
            *IdToPlayerPtr | std::views::keys | std::views::filter([this](const Id& id) {
                return (!IsLeader(id))
                    && (IsInGame(id))
                    && (IsAlive(id))
                    && (!IsMafia(id));
            }),
            std::back_inserter(ids)
        );

        std::cout << "Choose player to voite as mafia:" << std::endl;
        for (size_t i = 0; i < ids.size(); ++i) {
            std::cout << "Player " << ids[i] << std::endl;
        }

        Id choosenId;
        do {
            std::cout << "Choose id from list: ";
            std::cin >> choosenId;
        } while (std::count_if(ids.begin(), ids.end(), [choosenId](const Id& id) { return id == choosenId; }) == 0);
        return IdToPlayerPtr->at(choosenId);
    }
}
