#pragma once

#include "player_playable.h"


namespace NMafia {
    class TPlayerSheriff : public TPlayerPlayable {
    public:
        TPlayerSheriff(
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
                ERoles::Sheriff,
            }
        ) {}

        virtual PlayerAction NigthAction() override;

    protected:
        void Kill();
        void Check();
        TSharedPtr<TPlayerBase> ChooseTargetToKill();
        TSharedPtr<TPlayerBase> ChooseTargetToCheck();
        std::set<Id> CheckedIds;
    };
}
