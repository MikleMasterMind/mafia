#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerDoctor : public virtual TPlayerPlayable {
    public:
        TPlayerDoctor(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr
        )
        : TPlayerPlayable(
            idToPlayerPtr,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Doctor,
            }
        )
        {}

        virtual PlayerAction NigthAction() override;

    protected:
        virtual TSharedPtr<TPlayerBase> ChooseTargetToHeal();

    protected:
        Id PrevPacientId;
    };
}
