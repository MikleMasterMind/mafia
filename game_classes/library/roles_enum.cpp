#include "roles_enum.h"

#include <vector>
#include <random>


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

    ERoles GetRandomRole() {
        std::vector<ERoles> allRoles = {
            ERoles::Mafia,
            ERoles::Peacefull,
            ERoles::Civilian,
            ERoles::Doctor,
            ERoles::Sheriff,
            ERoles::Maniac
        };

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, allRoles.size() - 1);

        return allRoles[distrib(gen)];
    }
}
