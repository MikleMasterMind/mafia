#include "roles_enum.h"


namespace NMafia {
    std::string RoleToString(ERoles role) {
        switch (role) {
            case ERoles::Default:    return "Default";
            case ERoles::Leader:    return "Leader";
            case ERoles::Mafia:     return "Mafia";
            case ERoles::Peacefull: return "Peacefull";
            case ERoles::Civilian:  return "Civilian";
            case ERoles::Doctor:    return "Doctor";
            case ERoles::Sheriff:   return "Sheriff";
            case ERoles::Maniac:   return "Maniac";
            case ERoles::User:   return "User";
            default:               return "Unknown";
        }
    }
    std::string RolesToString(const std::set<ERoles> &roles) {
        if (roles.empty()) {
            return "";
        }

        std::stringstream stream;
        auto it = roles.begin();

        // Обрабатываем все роли, кроме последней
        for (; it != roles.end(); ++it) {
            if (std::next(it) != roles.end()) {
                stream << RoleToString(*it) << ", ";
            } else {
                // Для последней роли не добавляем запятую
                stream << RoleToString(*it);
            }
        }

        return stream.str();
    }
}
