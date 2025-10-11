#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerCivilian : public virtual TPlayerPlayable {
    public:
        TPlayerCivilian(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr
        )
        : TPlayerPlayable(
            idToPlayerPtr,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Civilian,
            }
        ) {}

        virtual PlayerAction NigthAction() override { co_return; }
    };
}
