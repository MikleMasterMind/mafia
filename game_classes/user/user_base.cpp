#include "user_base.h"

#include <iostream>
#include <thread>
#include <ranges>
#include <algorithm>

namespace NMafia {
    void TUserBase::Vote() {
        std::vector<Id> ids;
        std::ranges::copy(*IdToPlayerPtr | std::views::keys | std::views::filter([this](const auto& id) {
                return ((id != GetId())
                    && (IsInGame(id))
                    && (!IsLeader(id)));
                }),
            std::back_inserter(ids)
        );

        std::cout << "Choose player to vote again:" << std::endl;
        for (size_t i = 0; i < ids.size(); ++i) {
            std::cout << "Player " << ids[i] << std::endl;
        }

        Id choosenId;
        do {
            std::cout << "Choose id from list: ";
            std::cin >> choosenId;
        } while (std::count_if(ids.begin(), ids.end(), [&](const Id& id) { return id == choosenId; }) == 0);

        auto target = IdToPlayerPtr->at(choosenId);

        WriteMsgByRole(
            {
                {"message", "Vote again"},
                {"id", target->GetId()}
            },
            ERoles::Default
        );

        TLogger::Log(
            "Player " + GetId() + " vote again " + target->GetId()
        );
    }

    void TUserBase::ProcessSingleMessage(const TMessage& msg) {
        if (msg.Src == GetId()) {
            return;
        }
        std::string messageStr;
        if (IsLeader(msg.Src)) {
            messageStr = "=========\nFrom leader " + msg.Src + ": " + msg.Body.GetOrNull("announc") + "\n=========";
        } else {
            if (msg.Body.GetOrNull("message") == "Vote again") {
                auto targetId = msg.Body.Get("id") == GetId() ? "you" : msg.Body.Get("id");
                messageStr = "Player " + msg.Src + " vote again " + targetId;
            } else {
                messageStr = "From player " + msg.Src + ": " + msg.Body.ToString();
            }
        }
        MessageFileStream << messageStr << std::endl;
    }
}
