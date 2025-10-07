#pragma once

#include "../players/player_playable.h"
#include <queue>
#include <chrono>

namespace NMafia {
    class TUserBase : public TPlayerPlayable {
    public:
        TUserBase(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const std::vector<fs::path>& logPaths,
            const std::set<ERoles>& roles
        )
            : TPlayerPlayable(idToPlayerPtr, logPaths, roles) {
            lastMessageCheck = std::chrono::steady_clock::now();
        }

    protected:
        void Voite() override;

        void ProcessSingleMessage(const TMessage& msg) override;

    private:
        void PrintAccumulatedMessages();

        std::queue<TMessage> messagesQueue;
        std::chrono::steady_clock::time_point lastMessageCheck;
    };
}
