#pragma once

#include "user_base.h"
#include "../players/player_sheriff.h"

namespace NMafia {
    class TUserSheriff : public TUserBase, public TPlayerSheriff {
    public:
        TUserSheriff(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const fs::path& messageFilePath
        )
        : TUserBase(
            idToPlayerPtr,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Sheriff,
                ERoles::User,
            },
            messageFilePath
        )
        , TPlayerSheriff(
            idToPlayerPtr
        )
        , TPlayerPlayable(
            idToPlayerPtr,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Sheriff,
                ERoles::User,
            }
        )
        {}

    protected:
        PlayerAction NigthAction() override;

        void ProcessSingleMessage(const TMessage& msg) override {
            TUserBase::ProcessSingleMessage(msg);
        }

        void Voite() override {
            TUserBase::Voite();
        }

        void Kill() override;
        void Check() override;
        TSharedPtr<TPlayerBase> ChooseTargetToKill() override;
        TSharedPtr<TPlayerBase> ChooseTargetToCheck() override;
    };
}
