#pragma once

#include "user_base.h"


namespace NMafia {
    class TUserCivilian : public TUserBase {
    public:
        TUserCivilian(
            const TSharedPtr<std::unordered_map<Id, TSharedPtr<TPlayerBase>>>& idToPlayerPtr,
            const std::vector<fs::path>& logPaths,
            const std::set<ERoles>& roles,
            const fs::path& messageFilePath
        )
            : TUserBase(idToPlayerPtr, logPaths, roles, messageFilePath)
        {}

        virtual PlayerAction NigthAction() {
            co_return;
        }
    };
}
