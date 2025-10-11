#pragma once

#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <coroutine>
#include "../library/roles_enum.h"
#include "../library/message_reader.h"
#include "../library/message_writer.h"
#include "../../library/shared_ptr/shared_ptr.h"
#include "../../library/unique_number_generator/unique_number_generator.h"
#include "../../library/logger/logger.h"


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
        Excluded,
    };

    class TPlayerBase : public TMessageReader, public TMessageWriter {
    public:
        TPlayerBase(
            const std::set<ERoles>& roles,
            const TSharedPtr<TMessagesQueue>& queuePtr,
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr
        )
            : TMessageReader(queuePtr)
            , PersonChatPtr(queuePtr)
            , Roles(roles)
            , IdToPlayerPtr(idToPlayerPtr)
            , PersonId(std::to_string(UniqueNumberGenerator::getInstance().getUniqueNumber()))
        {
            TLogger::Log(
                "Initialize player role: " + RolesToString(GetRoles()) + " Id: " + GetId()
            );
        };

        TPlayerBase(const TPlayerBase& other)
            : TMessageReader(other.Queue)
            , PersonChatPtr(other.Queue)
            , Roles(other.Roles)
            , IdToPlayerPtr(other.IdToPlayerPtr)
            , PersonId(other.PersonId)
            , Status(EStatus::Alive)
        {
            TLogger::Log(
                "Initialize player role: " + RolesToString(GetRoles()) + " Id: " + GetId()
            );
        };

        virtual ~TPlayerBase() = default;

        TSharedPtr<TMessagesQueue> GetChat() const {
            return PersonChatPtr;
        }

        std::set<ERoles> GetRoles() const {
            return Roles;
        }

        Id GetId() const {
            return PersonId;
        }

        void StartProcessing() {
            TMessageReader::StartProcessing();
        }

        void StopProcessing() {
            TMessageReader::StopProcessing();
        }

        virtual PlayerAction DayAction() = 0;
        virtual PlayerAction NigthAction() = 0;

        EStatus GetStatus() const {
            return Status;
        }

        virtual void SetStatus(EStatus status) {
            Status = status;
        }

        bool IsLeader(Id id);
        bool IsInGame(Id id);
        bool IsAlive(Id id);
        bool IsMafia(Id id);

    protected:
        void WriteMsgById(const TJsonMap& msg, Id id);
        void WriteMsgByRole(const TJsonMap& msg, ERoles role);

    protected:
        TSharedPtr<TMessagesQueue> PersonChatPtr;
        std::set<ERoles> Roles;
        TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>> IdToPlayerPtr;
        Id PersonId;
        EStatus Status;
    };
}
