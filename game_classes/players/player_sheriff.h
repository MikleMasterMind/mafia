#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerSheriff : public TPlayerPlayable {
    public:
        TPlayerSheriff(
            const std::set<ERoles>& roles,
            const TSharedPtr<TMessagesQueue>& queuePtr,
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerPlayable>>>& idToPlayerPlayablePtr)
        : TPlayerPlayable(roles, queuePtr, idToPlayerPtr, idToPlayerPlayablePtr)
        {}

        virtual PlayerAction NigthAction() override;

    protected:
        TSharedPtr<TPlayerPlayable> ChooseTargetToKill();
        TSharedPtr<TPlayerPlayable> ChooseTargetToCheck();
    };
}
