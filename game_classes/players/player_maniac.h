#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerManiac : public virtual TPlayerPlayable {
    public:
        TPlayerManiac(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const std::vector<fs::path>& logPaths
        )
        : TPlayerPlayable(
            idToPlayerPtr,
            logPaths,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Maniac,
            }
        ) {}

        virtual PlayerAction NigthAction() override;

    protected:
        virtual TSharedPtr<TPlayerBase> ChooseTargetToKill();
    };
}
