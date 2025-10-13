#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerManiac : public virtual TPlayerPlayable {
    public:
        TPlayerManiac(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr
        )
        : TPlayerPlayable(
            idToPlayerPtr,
            {
                ERoles::Default,
                ERoles::Maniac,
            }
        ) {}

        virtual PlayerAction NigthAction() override;

    protected:
        virtual TSharedPtr<TPlayerBase> ChooseTargetToKill();
    };
}
