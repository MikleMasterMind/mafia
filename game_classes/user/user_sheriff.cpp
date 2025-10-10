#include "user_sheriff.h"

#include <iostream>
#include <ranges>

namespace NMafia {
    PlayerAction TUserSheriff::NigthAction() {
        std::string action;
        do {
            std::cout << "Choose action (check/kill): ";
            std::cin >> action;
        } while (action != "check" && action != "kill");

        if (action == "check") {
            Check();
        } else {
            Kill();
        }
        co_return;
    }

    void TUserSheriff::Kill() {
        TPlayerSheriff::Kill();
    }

    void TUserSheriff::Check() {
        auto target = ChooseTargetToCheck();
        if (!target) {
            Kill();
            return;
        }
        CheckedIds.insert(target->GetId());
        TLogger::multiLog(LogPaths,
            "Sheriff " + GetId() + " checks " + target->GetId()
        );
        if (IsMafia(target->GetId())) {
            std::cout << target->GetId() + " is mafia\n";
        } else {
            std::cout << target->GetId() + " is not mafia\n";
        }
    }

    TSharedPtr<TPlayerBase> TUserSheriff::ChooseTargetToKill()
    {
        std::vector<Id> ids;
        std::ranges::copy(
            *IdToPlayerPtr | std::views::keys | std::views::filter([this](const auto& id) {
                return (id != GetId())
                    && (!IsLeader(id))
                    && (IsAlive(id)
                    && (CheckedIds.find(id) == CheckedIds.end()));
            }),
            std::back_inserter(ids)
        );

        std::cout << "Choose player to kill:n";
        for (size_t i = 0; i < ids.size(); ++i) {
            std::cout << "Player " + ids[i] + "\n";
        }

        Id choosenId;
        do {
            std::cout << "Choose id from list: ";
            std::cin >> choosenId;
        } while (std::count_if(ids.begin(), ids.end(), [choosenId](const Id& id) { return id == choosenId; }) == 0);

        return IdToPlayerPtr->at(choosenId);
    }

    TSharedPtr<TPlayerBase> TUserSheriff::ChooseTargetToCheck() {
        std::vector<Id> ids;
        std::ranges::copy(
            *IdToPlayerPtr | std::views::keys | std::views::filter([this](const auto& id) {
                return (id != GetId())
                    && (!IsLeader(id))
                    && (IsAlive(id))
                    && (CheckedIds.find(id) == CheckedIds.end());
            }),
            std::back_inserter(ids)
        );

        if (ids.size() == 0) {
            std::cout << "No targets to check\n";
            return TSharedPtr<TPlayerBase>();
        }

        std::cout << "Choose player to check:\n";
        for (size_t i = 0; i < ids.size(); ++i) {
            std::cout << "Player " + ids[i] + "\n";
        }

        Id choosenId;
        do {
            std::cout << "Choose id from list: ";
            std::cin >> choosenId;
        } while (std::count_if(ids.begin(), ids.end(), [choosenId](const Id& id) {
            return id == choosenId;
        }) == 0);

        return IdToPlayerPtr->at(choosenId);
    }
}
