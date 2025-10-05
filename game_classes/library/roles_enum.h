#pragma once

#include <set>
#include <string>
#include <sstream>
#include <algorithm>


namespace NMafia {
    enum ERoles {
        Default,
        Leader,
        Mafia,
        Peacefull,
        Civilian,
        Doctor,
        Sheriff,
        Maniac
    };

    std::string RoleToString(ERoles role);

    std::string RolesToString(const std::set<ERoles>& roles);
}
