#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerMafia : public virtual TPlayerPlayable {
    public:
        TPlayerMafia(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr
        )
        : TPlayerPlayable(
            idToPlayerPtr,
            {
                ERoles::Default,
                ERoles::Mafia,
            }
        ) {}

        virtual PlayerAction NigthAction() override;

    protected:
        void MafiaVote();
        virtual TSharedPtr<TPlayerBase> ChooseTargetToMafiaVote();

        virtual TSharedPtr<TPlayerBase> ChooseTargretToVote() override;
    };
}
