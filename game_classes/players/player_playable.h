#pragma once

#include "player_base.h"
#include <unordered_map>
#include <coroutine>


namespace NMafia {
    struct PlayerAction {
        struct promise_type {
            PlayerAction get_return_object() {
                return PlayerAction{std::coroutine_handle<promise_type>::from_promise(*this)};
            }
            std::suspend_always initial_suspend() { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }
            void return_void() {}
            void unhandled_exception() {}
        };
        std::coroutine_handle<promise_type> handle;
        ~PlayerAction() { handle.destroy(); }
    };

    enum EStatus {
        Alive,
        Protected,
        Dead,
    };

    class TPlayerPlayable : public TPlayerBase {
    public:
        TPlayerPlayable(
            const std::set<ERoles>& roles,
            const TSharedPtr<TMessagesQueue>& queuePtr,
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerPlayable>>>& idToPlayerPlayablePtr)
        : IdToPlayerPlayablePtr(idToPlayerPlayablePtr)
        , TPlayerBase(roles, queuePtr, idToPlayerPtr)
        , Status(EStatus::Alive)
        {
            for (const auto& [id, player] : *IdToPlayerPtr) {
                auto roles = player->GetRoles();
                if (roles.find(ERoles::Leader) == roles.end()) {
                    TrustTable[id] = 0;
                }
            }
        }

        ~TPlayerPlayable() = default;

        virtual PlayerAction DayAction();
        virtual PlayerAction NigthAction() = 0;

        EStatus GetStatus() const {
            return Status;
        }

        void SetStatus(EStatus status) {
            Status = status;
        }

    protected:
        virtual void Voite();
        TSharedPtr<TPlayerBase> ChooseTargretToVoite();

        virtual void ProcessSingleMessage(const TMessage& msg) override;

    protected:
        TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerPlayable>>> IdToPlayerPlayablePtr;
        std::unordered_map<Id, int> TrustTable;
        EStatus Status;
    };
}
