#include "user_base.h"

#include <iostream>
#include <thread>
#include <ranges>
#include <algorithm>

namespace NMafia {
    void TUserBase::Voite() {
        std::vector<Id> ids;
        std::ranges::copy(*IdToPlayerPtr | std::views::keys, std::back_inserter(ids));

        std::cout << "Choose player to voite again:" << std::endl;
        for (size_t i = 0; i < ids.size(); ++i) {
            const auto& id = ids[i];
            if (id != GetId() && IsAlive(id) && IsInGame(id)) {
                std::cout << i + 1 << ". Player " << id
                          << " (trust: " << TrustTable[id] << ")" << std::endl;
            }
        }

        int choice;
        do {
            std::cout << "Choose id from list: ";
            std::cin >> choice;
        } while (choice < 1 || choice > static_cast<int>(ids.size()));

        Id choosenId = ids[choice - 1];
        auto target = IdToPlayerPtr->at(choosenId);

        WriteMsgByRole(
            {
                {"message", "Voite again"},
                {"id", target->GetId()}
            },
            ERoles::Default
        );

        TLogger::multiLog(LogPaths,
            "Player " + GetId() + " voite again " + target->GetId()
        );
    }

    void TUserBase::ProcessSingleMessage(const TMessage& msg) {
        messagesQueue.push(msg);

        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - lastMessageCheck).count() >= 5) {
            PrintAccumulatedMessages();
            lastMessageCheck = now;
        }
    }

    void TUserBase::PrintAccumulatedMessages() {
        while (!messagesQueue.empty()) {
            const auto& msg = messagesQueue.front();
            std::cout << "From player " << msg.Src << ": " << msg.Body.GetOrNull("message") << std::endl;
            messagesQueue.pop();
        }
        std::cout << std::endl;
    }
}
