#pragma once

#include "messages_queue.h"


namespace NMafia {
    class TMessageWriter {
    public:
       TMessageWriter() {}

        void Write(const TMessage& msg, TMessagesQueue& queue) {
            queue.Write(msg);
        }
    };
}
