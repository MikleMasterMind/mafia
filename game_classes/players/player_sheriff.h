#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerSheriff : public virtual TPlayerPlayable {
    public:
        TPlayerSheriff(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr
        )
        : TPlayerPlayable(
            idToPlayerPtr,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Sheriff,
            }
        ) {}

        virtual PlayerAction NigthAction() override;

    protected:
        virtual void Kill();
        virtual void Check();
        virtual TSharedPtr<TPlayerBase> ChooseTargetToKill();
        virtual TSharedPtr<TPlayerBase> ChooseTargetToCheck();

    protected:
        std::set<Id> CheckedIds;
    };
}
