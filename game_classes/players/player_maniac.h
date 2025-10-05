#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerManiac : public TPlayerPlayable {
    public:
        TPlayerManiac(
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
                ERoles::Maniac,
            }
        ) {}

        virtual PlayerAction NigthAction() override;

    protected:
        TSharedPtr<TPlayerBase> ChooseTargetToKill();
    };
}
