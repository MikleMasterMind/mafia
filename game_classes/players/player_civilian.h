#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerCivilian : TPlayerPlayable {
    public:
        TPlayerCivilian(
            const TSharedPtr<TMessagesQueue>& queuePtr,
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerPlayable>>>& idToPlayerPlayablePtr)
        : TPlayerPlayable(
            {
                ERoles::Peacefull,
                ERoles::Civilian,
            },
            queuePtr,
            idToPlayerPtr,
            idToPlayerPlayablePtr
        ) {}

        virtual PlayerAction NigthAction() override { co_return; }
    };
}
