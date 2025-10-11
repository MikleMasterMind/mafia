#include "logger.h"


namespace NMafia {
    std::ofstream TLogger::FileStream;
    fs::path TLogger::FilePath;
    std::mutex TLogger::WriteMutex;
    std::string TLogger::timestamp;
    bool TLogger::Enable = false;

    void TLogger::SetLogFilePath(const std::string& filename) {
        if (!Enable) {
            return;
        }

        std::unique_lock<std::mutex> lock(WriteMutex);
        if (FileStream.is_open()) {
            FileStream.close();
        }

        FilePath = "logs/" + timestamp + "/" + filename;

        if (!fs::exists(FilePath.parent_path())) {
            fs::create_directories(FilePath.parent_path());
        }

        FileStream.open(FilePath, std::ios::app);
    }

    void TLogger::Log(const std::string &message) {
        std::unique_lock<std::mutex> lock(WriteMutex);

        if (Enable && FileStream.is_open()) {
            FileStream << message << std::endl;
        }
    }

    void TLogger::SetEnable() {
        Enable = true;

        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm time = {};
        localtime_r(&in_time_t, &time);
        std::stringstream timestampStream;
        timestampStream << std::put_time(&time, "%y-%m-%d-%H-%M-%S");
        timestamp = timestampStream.str();
    }

    void TLogger::Destroy() {
        std::unique_lock<std::mutex> lock(WriteMutex);
        if (FileStream.is_open()) {
            FileStream.close();
        }
    }
}
