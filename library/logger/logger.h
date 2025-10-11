#pragma once

#include <string>
#include <mutex>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <iomanip>


namespace NMafia {

    namespace fs = std::filesystem;

    class TLogger {
    private:
        static std::ofstream FileStream;
        static fs::path FilePath;
        static std::mutex WriteMutex;
        static std::string timestamp;
        static bool Enable;

    public:
        static void SetLogFilePath(const std::string& filename);

        static void Log(const std::string& message);

        static void SetEnable();

        static void Destroy();
    };
}
