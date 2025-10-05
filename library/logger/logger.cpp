#include "logger.h"


namespace NMafia {
    std::map<fs::path, TLogger*> TLogger::Instance = {};
    std::mutex TLogger::InstanceMutex;
    std::mutex TLogger::WriteMutex;
}
