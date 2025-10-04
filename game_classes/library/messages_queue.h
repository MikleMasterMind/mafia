#pragma once

#include <queue>
#include <condition_variable>
#include "message.h"


namespace NMafia {
    class TMessagesQueue {
    public:
        void Write(const TMessage& msg);
        TMessage Read();

    protected:
        std::queue<TMessage> Queue;
        std::mutex Mutex;
        std::condition_variable CV;
    };

}
