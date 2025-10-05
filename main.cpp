#include <future>
#include "game_classes/players/player_civilian.h"
#include "game_classes/players/player_doctor.h"
#include "game_classes/players/player_sheriff.h"
#include "game_classes/players/player_maniac.h"


using namespace NMafia;

int main() {
    auto idToPlayer = TSharedPtr(new std::unordered_map<Id, TSharedPtr<TPlayerBase>>);
    auto civilian = TSharedPtr<TPlayerBase>(new TPlayerCivilian(
        TSharedPtr(new TMessagesQueue()),
        idToPlayer
    ));
    auto doctor = TSharedPtr<TPlayerBase>(new TPlayerDoctor(
        TSharedPtr(new TMessagesQueue()),
        idToPlayer
    ));
    auto sheriff = TSharedPtr<TPlayerBase>(new TPlayerSheriff(
        TSharedPtr(new TMessagesQueue()),
        idToPlayer
    ));
    auto maniac = TSharedPtr<TPlayerBase>(new TPlayerManiac(
        TSharedPtr(new TMessagesQueue()),
        idToPlayer
    ));
    for (const auto& player : {civilian, doctor, sheriff, maniac}) {
        (*idToPlayer)[player->GetId()] = player;
    }

    std::vector<std::future<void>> futures;
    for (auto& player : {civilian, doctor, sheriff, maniac}) {
        futures.push_back(std::async(std::launch::async, [&]() {
            player->DayAction().handle.resume();
        }));
    }

    for (auto& future : futures) {
        future.get();
    }
    return 0;
}
