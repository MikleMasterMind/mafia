#include "library/shared_ptr/shared_ptr.h"
#include "game_classes/players/player_base.h"
#include <thread>

using namespace NMafia;

int main() {
    auto PlayerTable = make_shared<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>();

    auto player1 = NMafia::make_shared<TPlayerBase>(std::set<ERoles>(), make_shared<TMessagesQueue>(), PlayerTable);
    auto player2 = NMafia::make_shared<TPlayerBase>(std::set<ERoles>(), make_shared<TMessagesQueue>(), PlayerTable);


    PlayerTable->insert({player1->GetId(), player1});
    PlayerTable->insert({player2->GetId(), player2});

    std::thread thread1([&]() { player1->StartProcessing(); });
    std::thread thread2([&]() { player2->StartProcessing(); });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    player1->WriteMsgById("hello from 1", player2->GetId());
    player2->WriteMsgById("hello from 2", player1->GetId());

    thread1.join();
    thread2.join();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    return 0;
}
