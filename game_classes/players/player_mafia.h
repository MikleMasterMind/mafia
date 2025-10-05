#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerMafia : public TPlayerPlayable {
    public:
        TPlayerMafia(
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
                ERoles::Mafia,
            }
        ) {}

        virtual PlayerAction NigthAction() override;

    protected:
        void MafiaVoite();
        TSharedPtr<TPlayerBase> ChooseTargetToMafiaVoite();
    };
}
