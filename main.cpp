#include <future>
#include <thread>
#include <chrono>
#include <iostream>
#include "library/args_parser/args_parser.h"
#include "game_classes/players/player_civilian.h"
#include "game_classes/players/player_doctor.h"
#include "game_classes/players/player_sheriff.h"
#include "game_classes/players/player_maniac.h"
#include "game_classes/players/player_mafia.h"
#include "game_classes/players/player_leader.h"
#include "game_classes/user/user_civilian.h"
#include "game_classes/user/user_doctor.h"
#include "game_classes/user/user_maniac.h"


using namespace NMafia;

int main(int argc, char* argv[]) {
    auto args = ParseArgs(argc, argv);
    if (!args.Filled) {
        return 1;
    }

    // Prepare game
    auto idToPlayer = TSharedPtr(new std::unordered_map<Id, TSharedPtr<TPlayerBase>>);
    std::vector<TSharedPtr<TPlayerBase>> players;
    #define INITIALIZE_PLAYER(class, logfile) \
        players.push_back(TSharedPtr<TPlayerBase>(new class( \
            idToPlayer, \
            {"logs/main.log", logfile} \
        )));

    INITIALIZE_PLAYER(TPlayerDoctor, "logs/doctor.log");
    INITIALIZE_PLAYER(TPlayerSheriff, "logs/sheriff.log");
    INITIALIZE_PLAYER(TPlayerManiac, "logs/maniac.log");
    for (int i = 0; i < args.PlayerCount / args.MafiaDivider; ++i) {
        INITIALIZE_PLAYER(TPlayerMafia, "logs/mafia.log");
    }
    int civilianCount = args.PlayerCount - (args.PlayerCount / args.MafiaDivider) - 3;
    if (args.UserInGame) {
        civilianCount--;
        players.push_back(TSharedPtr<TPlayerBase>(new TUserManiac(
            idToPlayer,
            {"logs/main.log", "logs/user.log"},
            "./user_chat.txt"
        )));
    }
    for (int i = 0; i < civilianCount; ++i) {
        INITIALIZE_PLAYER(TPlayerCivilian, "logs/civilian.log");
    }

    #undef INITIALIZE_PLAYER

    auto gameEnded = TSharedPtr<bool>(new bool(false));
    auto leader = TSharedPtr<TPlayerBase>(new TPlayerLeader(
        idToPlayer,
        {"logs/main.log", "logs/leader.log"},
        gameEnded
    ));
    players.push_back(leader);

    std::vector<std::thread> gameTreads;
    for (auto& player : players) {
        (*idToPlayer)[player->GetId()] = player;
        gameTreads.push_back(std::thread([&]() { player->StartProcessing(); }));
    }

    // main loop
    while (!*gameEnded) {
        auto dayFuture = std::async(std::launch::async, [&]() {
            auto action = leader->DayAction();
            action.handle.resume();
        });
        dayFuture.get();

        if (*gameEnded) {
            break;
        }

        auto nigthFuture = std::async(std::launch::async, [&]() {
            auto action = leader->NigthAction();
            action.handle.resume();
        });
        nigthFuture.get();
    }


    // finish
    std::this_thread::sleep_for(std::chrono::seconds(1));
    for (auto& player : players) {
        player->StopProcessing();
    }
    for (auto& thread : gameTreads) {
        thread.join();
    }
    TLogger::destroyAll();
    return 0;
}
