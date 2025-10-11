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

        bool GameEnded();

    protected:
        virtual void ProcessSingleMessage(const TMessage& msg) override;

        void ProcessDayVoiting();
        void ProcessMafiaVoiting();
        void ProcessSheriffKill();
        void ProcessManiacKill();
        void CleanDoctorHealing();

    protected:
        std::unordered_map<Id, int> DayVoiteTable;
        std::unordered_map<Id, int> MafiaVoiteTable;
        Id TargetToKillForSheriff;
        Id TargetToKillForManiac;
        std::string GameResult;
    };
}
