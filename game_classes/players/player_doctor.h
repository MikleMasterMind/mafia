#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerDoctor : public TPlayerPlayable {
    public:
        TPlayerDoctor(
            const TSharedPtr<TMessagesQueue>& queuePtr,
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerPlayable>>>& idToPlayerPlayablePtr,
            const std::set<ERoles>& /* roles */
        )
        : TPlayerPlayable(
            queuePtr,
            idToPlayerPtr,
            idToPlayerPlayablePtr,
            {
                ERoles::Peacefull,
                ERoles::Doctor
            }
        )
        {}

        virtual PlayerAction NigthAction() override;

    protected:
        TSharedPtr<TPlayerPlayable> ChooseTargetToHeal();

    protected:
        Id PrevPacientId;
    };
}
