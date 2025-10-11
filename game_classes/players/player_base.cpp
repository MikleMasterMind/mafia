#include "player_base.h"
#include <ranges>
#include <iostream>

namespace NMafia {
    void TPlayerBase::WriteMsgById(const TJsonMap &msg, Id id){
        auto dest = IdToPlayerPtr->find(id);
        if (dest != IdToPlayerPtr->end()) {
            Write(TMessage{GetId(), msg}, *(dest->second->GetChat()));
        } else {
        }
    }

    void TPlayerBase::WriteMsgByRole(const TJsonMap &msg, ERoles role) {
        for (const auto& [id, player] : *IdToPlayerPtr | std::views::filter([&role, this](auto p) {
                auto roles = p.second->GetRoles();
                return (roles.find(role) != roles.end())
                    && (p.first != GetId());
            })
        ) {
            WriteMsgById(msg, id);
        }
    }

    bool TPlayerBase::IsLeader(Id id) {
        auto roles = IdToPlayerPtr->at(id)->GetRoles();
        return roles.find(ERoles::Leader) != roles.end();
    }

    bool TPlayerBase::IsInGame(Id id) {
        return IdToPlayerPtr->at(id)->GetStatus() != EStatus::Excluded;
    }

    bool TPlayerBase::IsAlive(Id id) {
        auto status = IdToPlayerPtr->at(id)->GetStatus();
        return  (status == EStatus::Alive) || (status == EStatus::Protected);
    }

    bool TPlayerBase::IsMafia(Id id) {
        auto roles = IdToPlayerPtr->at(id)->GetRoles();
        return roles.find(ERoles::Mafia) != roles.end();
    }

    std::string TPlayerBase::GetStatistic() const {
        std::stringstream stats;
        stats << "------------------------------------------------\n";
        stats << "ID: " << GetId() << "\n";
        stats << "Status: " <<
            (Status == EStatus::Alive ? "Alive" :
              (Status == EStatus::Dead ? "Dead" : "Excluded")) << "\n";
        stats << "Roles: " << RolesToString(GetRoles()) << "\n";
        return stats.str();
    }
}
