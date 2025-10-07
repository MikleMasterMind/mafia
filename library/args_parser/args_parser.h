#pragma once

#include <iostream>
#include <string>
#include <vector>


namespace NMafia {
    struct TParsedArgs {
        int PlayerCount;
        int MafiaDivider;
        bool UserInGame;
        bool DoLog;
        bool Filled;
    };

    TParsedArgs ParseArgs(int argc, char* argv[]);
}
