#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerCivilian : TPlayerPlayable {
        TPlayerCivilian(
            const std::set<ERoles>& roles,
            const TSharedPtr<TMessagesQueue>& queuePtr,
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr)
        : TPlayerPlayable(roles, queuePtr, idToPlayerPtr)
        {}

        virtual PlayerAction NigthAction() override { co_return; }
    };
}
