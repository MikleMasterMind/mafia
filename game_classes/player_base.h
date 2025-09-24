#pragma once

#include <vector>
#include <string>
#include <set>
#include "roles_enum.h"
#include "message.h"
#include "library/shared_ptr/shared_ptr.h"


namespace NMafia {
    class TPlayerBase {
    public:
        using Chat = std::vector<TMessage>;

        TPlayerBase(std::set<ERoles> roles)
            : Roles(std::move(roles))
        {
        };
        virtual ~TPlayerBase() = 0;

        virtual ReadMsg() = 0;
        virtual WriteMsg() = 0;

        TSharedPtr<Chat> GetChat() {
            return PrivateChat;
        }
    protected:
        std::set<ERoles> Roles;
        TSharedPtr<Chat> PrivateChat;
    }
}
