#include "args_parser.h"


namespace NMafia {
    TParsedArgs ParseArgs(int argc, char *argv[]) {
        TParsedArgs args{
            .PlayerCount = 4,
            .MafiaDivider = 3,
            .UserInGame = false,
            .DoLog = false,
            .Filled = false
        };

        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            if (arg == "-n") {
                if (i + 1 < argc) {
                    try {
                        args.PlayerCount = std::stoi(argv[i + 1]);
                        i++;
                    } catch (...) {
                        std::cerr << "Ошибка: неверное значение для -n\n";
                        return args;
                    }
                } else {
                    std::cerr << "Ошибка: отсутствует значение для -n\n";
                    return args;
                }
            } else if (arg == "-k") {
                if (i + 1 < argc) {
                    try {
                        args.MafiaDivider = std::stoi(argv[i + 1]);
                        i++;
                    } catch (...) {
                        std::cerr << "Ошибка: неверное значение для -k\n";
                        return args;
                    }
                } else {
                    std::cerr << "Ошибка: отсутствует значение для -k\n";
                    return args;
                }
            } else if (arg == "-u") {
                args.UserInGame = true;
            } else if (arg == "-l") {
                args.DoLog = true;
            } else {
                std::cerr << "Ошибка: неизвестный параметр " << arg << "\n";
                return args;
            }
        }

        args.Filled = true;
        return args;
    }
}
