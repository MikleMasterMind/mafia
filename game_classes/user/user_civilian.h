#pragma once

#include "user_base.h"
#include "../players/player_civilian.h"


namespace NMafia {
    class TUserCivilian : public virtual TUserBase, public virtual TPlayerCivilian {
    public:
        TUserCivilian(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const fs::path& messageFilePath
        )
        : TUserBase(
            idToPlayerPtr,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Civilian,
                ERoles::User,
            },
            messageFilePath
        )
        , TPlayerCivilian(
            idToPlayerPtr
        )
        , TPlayerPlayable(
            idToPlayerPtr,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Doctor,
                ERoles::User,
            }
        )
        {}

        virtual PlayerAction NigthAction() {
            return TPlayerCivilian::NigthAction();
        }
    };
}
