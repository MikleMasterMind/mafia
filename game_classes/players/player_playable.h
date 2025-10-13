#pragma once

#include "player_base.h"
#include <unordered_map>


namespace NMafia {
    class TPlayerPlayable : public TPlayerBase {
    public:
        TPlayerPlayable(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const std::set<ERoles>& roles
        )
        : TPlayerBase(
            roles,
            TSharedPtr(new TMessagesQueue()),
            idToPlayerPtr
        )
        {
            for (const auto& [id, player] : *IdToPlayerPtr) {
                auto roles = player->GetRoles();
                if (roles.find(ERoles::Leader) == roles.end()) {
                    std::lock_guard lock(TrustTableMutex);
                    TrustTable[id] = 0;
                }
            }
        }

        virtual ~TPlayerPlayable() = default;

        virtual PlayerAction DayAction() override;

    protected:
        virtual void Vote();
        virtual TSharedPtr<TPlayerBase> ChooseTargretToVote();

        virtual void ProcessSingleMessage(const TMessage& msg) override;

    protected:
        std::unordered_map<Id, int> TrustTable;
        std::mutex TrustTableMutex;
    };
}
