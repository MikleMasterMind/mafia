#pragma once

#include <string>
#include "../../library/json/json.h"

namespace NMafia {
    using Id = std::string;

    struct TMessage {
        Id Src;
        TJsonMap Body;
    };
}
