#pragma once

#include <string>

namespace NMafia {
    enum EMessageType {
        Default,
    }
    using Id = std::string;

    struct TMessage {
        EMessageType Type;
        Id Src;
        std::string Body;
    }
}
