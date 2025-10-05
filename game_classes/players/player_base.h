#pragma once

#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include "../library/roles_enum.h"
#include "../library/message_reader.h"
#include "../library/message_writer.h"
#include "../../library/shared_ptr/shared_ptr.h"
#include "../../library/unique_number_generator/unique_number_generator.h"


namespace NMafia {
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
        {};

        TPlayerBase(const TPlayerBase& other)
            : TMessageReader(other.Queue)
            , PersonChatPtr(other.Queue)
            , Roles(other.Roles)
            , IdToPlayerPtr(other.IdToPlayerPtr)
            , PersonId(other.PersonId)
        {};

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

    protected:
        void WriteMsgById(const TJsonMap& msg, Id id);
        void WriteMsgByRole(const TJsonMap& msg, ERoles role);

    protected:
        TSharedPtr<TMessagesQueue> PersonChatPtr;
        std::set<ERoles> Roles;
        TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>> IdToPlayerPtr;
        Id PersonId;
    };
}
