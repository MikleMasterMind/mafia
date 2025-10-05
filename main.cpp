#include <future>
#include <thread>
#include <chrono>
#include "game_classes/players/player_civilian.h"
#include "game_classes/players/player_doctor.h"
#include "game_classes/players/player_sheriff.h"
#include "game_classes/players/player_maniac.h"
#include "game_classes/players/player_mafia.h"


using namespace NMafia;

int main() {
    std::vector<fs::path> paths = {
        "logs/main.log",
    };
    TLogger::multiLog(paths, "\n\n==========================\n");
    auto idToPlayer = TSharedPtr(new std::unordered_map<Id, TSharedPtr<TPlayerBase>>);
    std::vector<TSharedPtr<TPlayerBase>> players;
    players.push_back(TSharedPtr<TPlayerBase>(new TPlayerCivilian(
        TSharedPtr(new TMessagesQueue()),
        idToPlayer,
        paths
    )));
    players.push_back(TSharedPtr<TPlayerBase>(new TPlayerDoctor(
        TSharedPtr(new TMessagesQueue()),
        idToPlayer,
        paths
    )));
    players.push_back(TSharedPtr<TPlayerBase>(new TPlayerSheriff(
        TSharedPtr(new TMessagesQueue()),
        idToPlayer,
        paths
    )));
    players.push_back(TSharedPtr<TPlayerBase>(new TPlayerManiac(
        TSharedPtr(new TMessagesQueue()),
        idToPlayer,
        paths
    )));
    players.push_back(TSharedPtr<TPlayerBase>(new TPlayerMafia(
        TSharedPtr(new TMessagesQueue()),
        idToPlayer,
        paths
    )));

    std::vector<std::thread> gameTreads;
    for (auto& player : players) {
        (*idToPlayer)[player->GetId()] = player;
        gameTreads.push_back(std::thread([&]() { player->StartProcessing(); }));
    }

    std::vector<std::future<void>> futures;
    for (auto& player : players) {
        futures.push_back(std::async(std::launch::async, [&]() {
            player->DayAction().handle.resume();
        }));
    }

    for (auto& future : futures) {
        future.get();
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    for (auto& player : players) {
        player->StopProcessing();
    }
    for (auto& thread : gameTreads) {
        thread.join();
    }
    TLogger::destroyAll();
    return 0;
}
