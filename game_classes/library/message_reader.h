#pragma once

#include "messages_queue.h"
#include "../../library/shared_ptr/shared_ptr.h"


namespace NMafia {
    class TMessageReader {
    public:
        TMessageReader(const TSharedPtr<TMessagesQueue>& queue) : Queue(queue) {}

        void StartProcessing() {
            ProcessMessages();
        }

    protected:
        void ProcessMessages() {
            while (true) {
                TMessage message = Queue->Read();
                if (!message.Body.Size()) {
                    continue;
                }
                ProcessSingleMessage(message);
            }
        }

        virtual void ProcessSingleMessage(const TMessage& msg) = 0;

        TSharedPtr<TMessagesQueue> Queue;
    };
}
