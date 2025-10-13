#pragma once

#include "player_base.h"


namespace NMafia {
    class TPlayerLeader : public TPlayerBase {
    public:
        TPlayerLeader(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr
        )
        : TPlayerBase(
            {
                ERoles::Default,
                ERoles::Leader,
            },
            TSharedPtr(new TMessagesQueue()),
            idToPlayerPtr
          )
        {}

        virtual PlayerAction DayAction() override;
        virtual PlayerAction NigthAction() override;

        void SayResult();

        void CountPlayers();

        bool GameEnded();

    protected:
        virtual void ProcessSingleMessage(const TMessage& msg) override;

        void ProcessDayVoting();
        void ProcessMafiaVoting();
        void ProcessSheriffKill();
        void ProcessManiacKill();
        void CleanDoctorHealing();

    protected:
        std::unordered_map<Id, int> DayVoteTable;
        std::unordered_map<Id, int> MafiaVoteTable;
        Id TargetToKillForSheriff;
        Id TargetToKillForManiac;
        std::string GameResult;
        int PeacefullCount;
        int MafiaCount;
        int ManiacCount;
    };
}
