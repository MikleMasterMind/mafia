#include "messages_queue.h"


namespace NMafia {
    void TMessagesQueue::Write(const TMessage &msg) {
        std::unique_lock<std::mutex> lock(Mutex);
        Queue.push(msg);
        CV.notify_one();
    }

    TMessage TMessagesQueue::Read() {
        std::unique_lock<std::mutex> lock(Mutex);
        CV.wait(lock, [this] { return !Queue.empty(); });
        TMessage msg = Queue.front();
        Queue.pop();
        return msg;
    }
}
