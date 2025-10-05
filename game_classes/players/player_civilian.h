#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerCivilian : public TPlayerPlayable {
    public:
        TPlayerCivilian(
            const TSharedPtr<TMessagesQueue>& queuePtr,
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const std::set<ERoles>& /* roles */ = {}
        )
        : TPlayerPlayable(
            queuePtr,
            idToPlayerPtr,
            {
                ERoles::Peacefull,
                ERoles::Civilian,
            }
        ) {}

        virtual PlayerAction NigthAction() override { co_return; }
    };
}
