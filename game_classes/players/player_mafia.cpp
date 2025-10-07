#include "player_mafia.h"

#include <ranges>


namespace NMafia {
    PlayerAction TPlayerMafia::NigthAction() {
        MafiaVoite();
        co_return;
    }

    void TPlayerMafia::MafiaVoite() {
        auto target = ChooseTargetToMafiaVoite();
        WriteMsgByRole(
            {
                {"message", "Mafia voite again"},
                {"id", target->GetId()},
            },
            ERoles::Leader
        );
        TLogger::multiLog(LogPaths,
            "Player " + GetId() + " voite as mafia again " + target->GetId()
        );
    }

    TSharedPtr<TPlayerBase> TPlayerMafia::ChooseTargetToMafiaVoite() {
        std::vector<Id> ids;
        std::ranges::copy(*IdToPlayerPtr | std::views::keys std::veiwsfilter([this](const Id& id) {
                return id != GetId();
            }),
            std::back_inserter(ids)
        );

        int minTrust = std::ranges::min(
            ids | std::views::transform([this](const Id& id) {
                return TrustTable[id];
            })
        );
        int threshold = minTrust * 0.85;

        std::vector<Id> billKill;
        std::ranges::copy(
            ids | std::views::filter([this, threshold](const Id& id) {
                return (TrustTable[id] <= threshold)
                    && (!IsLeader(id))
                    && (IsInGame(id))
                    && (IsAlive(id))
                    && (IsMafia(id));
            }),
            std::back_inserter(billKill)
        );

        std::random_device rd;
        std::mt19937 gen(rd());
        Id choosenId;
        if (billKill.empty()) {;
            choosenId = ids[std::uniform_int_distribution<>(0, ids.size() - 1)(gen)];
        } else {
            choosenId = billKill[std::uniform_int_distribution<>(0, billKill.size() - 1)(gen)];
        }

        return IdToPlayerPtr->at(choosenId);
    }
}
