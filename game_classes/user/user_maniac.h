#pragma once

#include "user_base.h"
#include "../players/player_maniac.h"

namespace NMafia {
    class TUserManiac : public TUserBase, public TPlayerManiac {
    public:
        TUserManiac(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const fs::path& messageFilePath
        )
        : TUserBase(
            idToPlayerPtr,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Maniac,
                ERoles::User,
            },
            messageFilePath)
        , TPlayerManiac(
            idToPlayerPtr
        )
        , TPlayerPlayable(
            idToPlayerPtr,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Maniac,
                ERoles::User,
            }
        )

        {}

    protected:
        PlayerAction NigthAction() override {
            return TPlayerManiac::NigthAction();
        }

        void ProcessSingleMessage(const TMessage& msg) override {
            TUserBase::ProcessSingleMessage(msg);
        }

        void Voite() override {
            TUserBase::Voite();
        }

        virtual TSharedPtr<TPlayerBase> ChooseTargetToKill() override;
    };
}
