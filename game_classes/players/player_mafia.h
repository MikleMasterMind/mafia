#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerMafia : public virtual TPlayerPlayable {
    public:
        TPlayerMafia(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const std::vector<fs::path>& logPaths
        )
        : TPlayerPlayable(
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

        virtual TSharedPtr<TPlayerBase> ChooseTargetToVoite() override;
    };
}
