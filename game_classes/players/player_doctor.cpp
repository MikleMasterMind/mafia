#include "player_doctor.h"

#include <vector>
#include <ranges>
#include <random>
#include <algorithm>


namespace NMafia {
    PlayerAction TPlayerDoctor::NigthAction() {
        auto target = ChooseTargetToHeal();
        auto targetStatus = target->GetStatus();
        if (targetStatus == EStatus::Alive || targetStatus == EStatus::Dead) {
            target->SetStatus(EStatus::Protected);
        }
        co_return;
    }

    TSharedPtr<TPlayerPlayable> TPlayerDoctor::ChooseTargetToHeal() {
        std::vector<Id> ids;
        std::ranges::copy(*IdToPlayerPlayablePtr | std::views::keys, std::back_inserter(ids));

        std::random_device rd;
        std::mt19937 gen(rd());
        Id choosenId;
        while (PrevPacientId.size() && PrevPacientId != choosenId) {
            choosenId = ids[std::uniform_int_distribution<>(0, ids.size() - 1)(gen)];
        }
        PrevPacientId = choosenId;

        return IdToPlayerPlayablePtr->at(choosenId);
    }
}
