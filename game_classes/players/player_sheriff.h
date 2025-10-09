#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerSheriff : public virtual TPlayerPlayable {
    public:
        TPlayerSheriff(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const std::vector<fs::path>& logPaths
        )
        : TPlayerPlayable(
            idToPlayerPtr,
            logPaths,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Sheriff,
            }
        ) {}

        virtual PlayerAction NigthAction() override;

    protected:
        void Kill();
        void Check();
        TSharedPtr<TPlayerBase> ChooseTargetToKill();
        TSharedPtr<TPlayerBase> ChooseTargetToCheck();

    protected:
        std::set<Id> CheckedIds;
    };
}
