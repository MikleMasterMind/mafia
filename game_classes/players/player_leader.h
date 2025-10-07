#pragma once

#include "player_base.h"


namespace NMafia {
    class TPlayerLeader : public TPlayerBase {
    public:
        TPlayerLeader(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const std::vector<fs::path>& logPaths,
            const TSharedPtr<bool> gameEnded
        )
        : TPlayerBase(
            {
                ERoles::Default,
                ERoles::Leader,
            },
            TSharedPtr(new TMessagesQueue()),
            idToPlayerPtr,
            logPaths
          )
        , GameEnded(gameEnded)
        {}

        virtual PlayerAction DayAction() override;
        virtual PlayerAction NigthAction() override;

    protected:
        virtual void ProcessSingleMessage(const TMessage& msg) override;

        void ProcessDayVoiting();
        void ProcessMafiaVoiting();
        void ProcessSheriffKill();
        void ProcessManiacKill();
        void CleanDoctorHealing();

        void CheckGameEnded();

    protected:
        std::unordered_map<Id, int> DayVoiteTable;
        std::unordered_map<Id, int> MafiaVoiteTable;
        Id TargetToKillForSheriff;
        Id TargetToKillForManiac;
        TSharedPtr<bool> GameEnded;
    };
}
