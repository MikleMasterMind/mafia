#pragma once

#include "../players/player_playable.h"
#include <queue>
#include <chrono>
#include <filesystem>
#include <iostream>


namespace NMafia {

    namespace fs = std::filesystem;

    class TUserBase : public virtual TPlayerPlayable {
    public:
        TUserBase(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const std::set<ERoles>& roles,
            const fs::path& messageFilePath
        )
            : TPlayerPlayable(
                idToPlayerPtr,
                roles
            )
            , MessageFile(messageFilePath)
        {
            fs::create_directories(MessageFile.parent_path());
            MessageFileStream.open(MessageFile, std::ios::app);
            if (!MessageFileStream.is_open()) {
                throw std::runtime_error("Cannot open message file: " + MessageFile.string());
            }
            WriteToView("<<<<< New Game >>>>>");
            WriteToView("Your id in game: " + GetId());
            WriteToView("Your roles in game: " + RolesToString(GetRoles()));
        }

        ~TUserBase() {
            if (MessageFileStream.is_open()) {
                MessageFileStream.close();
            }
        }

        virtual void SetStatus(EStatus status) override {
            TPlayerBase::SetStatus(status);
            if (!IsInGame(GetId())) {
                WriteToView("=========\nYou're not in game any more\n=========");
            }
        }

        void WriteToView(const std::string& msg) {
            MessageFileStream << msg << std::endl;
        }

    protected:
        void Vote() override;

        void ProcessSingleMessage(const TMessage& msg) override;

    protected:
        fs::path MessageFile;
        std::ofstream MessageFileStream;
    };
}
