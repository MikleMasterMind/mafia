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
#include "game_classes/user/user_sheriff.h"
#include "game_classes/user/user_mafia.h"

using namespace NMafia;

int main(int argc, char* argv[]) {
    auto args = ParseArgs(argc, argv);
    if (!args.Filled) {
        return 1;
    }

    // Prepare game
    auto idToPlayer = TSharedPtr(new std::unordered_map<Id, TSharedPtr<TPlayerBase>>);
    std::vector<TSharedPtr<TPlayerBase>> players;

    TSharedPtr<TPlayerBase> userPlayer;
    bool isUserCivilian = false;
    bool isUserDoctor = false;
    bool isUserSheriff = false;
    bool isUserManiac = false;
    bool isUserMafia = false;

    int mafiaCount = args.PlayerCount / args.MafiaDivider;
    int civilianCount = args.PlayerCount - mafiaCount - 3;

    if (args.UserInGame) {
        ERoles userRole = GetRandomRole();
        switch (userRole) {
            case ERoles::Civilian:
                userPlayer = TSharedPtr<TPlayerBase>(new TUserCivilian(
                    idToPlayer,
                    {"logs/main.log", "logs/user.log"},
                    "./user_chat.txt"
                ));
                isUserCivilian = true;
                break;
            case ERoles::Doctor:
                userPlayer = TSharedPtr<TPlayerBase>(new TUserDoctor(
                    idToPlayer,
                    {"logs/main.log", "logs/user.log"},
                    "./user_chat.txt"
                ));
                isUserDoctor = true;
                break;
            case ERoles::Sheriff:
                userPlayer = TSharedPtr<TPlayerBase>(new TUserSheriff(
                    idToPlayer,
                    {"logs/main.log", "logs/user.log"},
                    "./user_chat.txt"
                ));
                isUserSheriff = true;
                break;
            case ERoles::Maniac:
                userPlayer = TSharedPtr<TPlayerBase>(new TUserManiac(
                    idToPlayer,
                    {"logs/main.log", "logs/user.log"},
                    "./user_chat.txt"
                ));
                isUserManiac = true;
                break;
            case ERoles::Mafia:
                userPlayer = TSharedPtr<TPlayerBase>(new TUserMafia(
                    idToPlayer,
                    {"logs/main.log", "logs/user.log"},
                    "./user_chat.txt"
                ));
                isUserMafia = true;
                break;
            default:
                return 1;
        }
        players.push_back(userPlayer);
    }

    #define INITIALIZE_PLAYER(class) \
        players.push_back(TSharedPtr<TPlayerBase>(new class( \
            idToPlayer, \
            {"logs/main.log"} \
        )));

    if (!isUserDoctor) {
        INITIALIZE_PLAYER(TPlayerDoctor);
    }
    if (!isUserSheriff) {
        INITIALIZE_PLAYER(TPlayerSheriff);
    }
    if (!isUserManiac) {
        INITIALIZE_PLAYER(TPlayerManiac);
    }
    if (isUserCivilian) {
        --civilianCount;
    }
    for (int i = 0; i < civilianCount; ++i) {
        INITIALIZE_PLAYER(TPlayerCivilian);
    }
    if (isUserMafia) {
        --mafiaCount;
    }
    for (int i = 0; i < mafiaCount; ++i) {
        INITIALIZE_PLAYER(TPlayerMafia);
    }

    #undef INITIALIZE_PLAYER

    auto gameEnded = TSharedPtr<bool>(new bool(false));
    auto leader = TSharedPtr<TPlayerBase>(new TPlayerLeader(
        idToPlayer,
        {"logs/main.log"},
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
        auto nigthFuture = std::async(std::launch::async, [&]() {
            auto action = leader->NigthAction();
            action.handle.resume();
        });
        nigthFuture.get();

        if (*gameEnded) {
            break;
        }

        auto dayFuture = std::async(std::launch::async, [&]() {
            auto action = leader->DayAction();
            action.handle.resume();
        });
        dayFuture.get();
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
