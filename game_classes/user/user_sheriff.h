#pragma once

#include "user_base.h"
#include "../players/player_sheriff.h"

namespace NMafia {
    class TUserSheriff : public TUserBase, public TPlayerSheriff {
    public:
        TUserSheriff(
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
                ERoles::Sheriff,
                ERoles::User,
            },
            messageFilePath
        )
        , TPlayerSheriff(
            idToPlayerPtr,
            logPaths
        )
        , TPlayerPlayable(
            idToPlayerPtr,
            logPaths,
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
