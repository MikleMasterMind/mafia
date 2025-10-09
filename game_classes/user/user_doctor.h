#pragma once

#include "user_base.h"
#include "../players/player_doctor.h"

namespace NMafia {
    class TUserDoctor : public TUserBase, public TPlayerDoctor {
    public:
        TUserDoctor(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const std::vector<fs::path>& logPaths,
            const fs::path& messageFilePath
        )
        : TUserBase(
            idToPlayerPtr,
            logPaths,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Doctor,
                ERoles::User,
            },
            messageFilePath
        )
        , TPlayerDoctor(
            idToPlayerPtr,
            logPaths
        )
        , TPlayerPlayable(
            idToPlayerPtr,
            logPaths,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Doctor,
                ERoles::User,
            }
        )
        {}

    protected:
        PlayerAction NigthAction() override {
            return TPlayerDoctor::NigthAction();
        }

        void ProcessSingleMessage(const TMessage& msg) override {
            TUserBase::ProcessSingleMessage(msg);
        }

        void Voite() override {
            TUserBase::Voite();
        }

        virtual TSharedPtr<TPlayerBase> ChooseTargetToHeal() override;
    };
}
