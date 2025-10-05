#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerDoctor : public TPlayerPlayable {
    public:
        TPlayerDoctor(
            const TSharedPtr<TMessagesQueue>& queuePtr,
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const std::vector<fs::path>& logPaths,
            const std::set<ERoles>& /* roles */ = {}
        )
        : TPlayerPlayable(
            queuePtr,
            idToPlayerPtr,
            logPaths,
            {
                ERoles::Default,
                ERoles::Peacefull,
                ERoles::Doctor,
            }
        )
        {}

        virtual PlayerAction NigthAction() override;

    protected:
        TSharedPtr<TPlayerBase> ChooseTargetToHeal();

    protected:
        Id PrevPacientId;
    };
}
