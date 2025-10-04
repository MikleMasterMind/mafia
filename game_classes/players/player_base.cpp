#include "player_base.h"
#include <ranges>
#include <iostream>

namespace NMafia {
    void TPlayerBase::WriteMsgById(const std::string &msg, Id id){
        auto dest = IdToPlayerPtr->find(id);
        if (dest != IdToPlayerPtr->end()) {
            Write(TMessage{GetId(), msg}, *(dest->second->GetChat()));
        } else {
        }
    }

    void TPlayerBase::WriteMsgByRole(const std::string &msg, ERoles role) {
        for (auto& [id, player]
            : *IdToPlayerPtr
            | std::views::filter([&role](auto p) {
                return p.second->GetRoles().find(role) != p.second->GetRoles().end();
            })
        ) {
            WriteMsgById(msg, player->GetId());
        }
    }

    void TPlayerBase::ProcessSingleMessage(const TMessage &msg) {
        std::cout << "Player base id " << GetId() << " message: " << msg.Body << std::endl;
    }
}
