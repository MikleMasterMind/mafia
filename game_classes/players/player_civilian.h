#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerCivilian : public TPlayerPlayable {
    public:
        TPlayerCivilian(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const std::vector<fs::path>& logPaths
        )
        : TPlayerPlayable(
            idToPlayerPtr,
            logPaths,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Civilian,
            }
        ) {}

        virtual PlayerAction NigthAction() override { co_return; }
    };
}
