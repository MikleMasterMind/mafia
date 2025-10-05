#pragma once

#include "player_base.h"
#include <unordered_map>


namespace NMafia {
    class TPlayerPlayable : public TPlayerBase {
    public:
        TPlayerPlayable(
            const TSharedPtr<TMessagesQueue>& queuePtr,
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const std::set<ERoles>& roles
        )
        : TPlayerBase(roles, queuePtr, idToPlayerPtr)
        , Status(EStatus::Alive)
        {
            for (const auto& [id, player] : *IdToPlayerPtr) {
                auto roles = player->GetRoles();
                if (roles.find(ERoles::Leader) == roles.end()) {
                    TrustTable[id] = 0;
                }
            }
        }

        ~TPlayerPlayable() = default;

        virtual PlayerAction DayAction() override;

        virtual EStatus GetStatus() const override {
            return Status;
        }

        virtual void SetStatus(EStatus status) override {
            Status = status;
        }

    protected:
        virtual void Voite();
        TSharedPtr<TPlayerBase> ChooseTargretToVoite();

        virtual void ProcessSingleMessage(const TMessage& msg) override;

    protected:
        std::unordered_map<Id, int> TrustTable;
        EStatus Status;
    };
}
