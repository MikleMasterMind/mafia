#pragma once

#include "user_base.h"
#include "../players/player_mafia.h"

namespace NMafia {
    class TUserMafia : public TUserBase, public TPlayerMafia {
    public:
        TUserMafia(
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
                ERoles::Mafia,
                ERoles::User,
            },
            messageFilePath
        )
        , TPlayerMafia(
            idToPlayerPtr,
            logPaths
        )
        , TPlayerPlayable(
            idToPlayerPtr,
            logPaths,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Mafia,
                ERoles::User,
            }
        )
        , FirstNigth(true)
        {}

    protected:

        PlayerAction NigthAction() override;

        void ProcessSingleMessage(const TMessage& msg) override {
            TUserBase::ProcessSingleMessage(msg);
        }

        void Voite() override {
            TUserBase::Voite();
        }

        TSharedPtr<TPlayerBase> ChooseTargetToMafiaVoite() override;

    protected:
        bool FirstNigth;
    };
}
