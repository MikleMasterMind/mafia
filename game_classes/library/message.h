#pragma once

#include <string>

namespace NMafia {
    using Id = std::string;

    struct TMessage {
        Id Src;
        std::string Body;
    };
}
