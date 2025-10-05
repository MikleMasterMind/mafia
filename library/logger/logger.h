#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <map>
#include <mutex>
#include <iostream>
#include <vector>

namespace NMafia {

    namespace fs = std::filesystem;

    class TLogger {
    private:
        static std::map<fs::path, TLogger*> Instance;
        static std::mutex InstanceMutex;
        static std::mutex WriteMutex;
        std::ofstream FileStream;
        fs::path FilePath;

        TLogger(const fs::path& path) : FilePath(path) {
            try {
                if (!fs::exists(FilePath.parent_path())) {
                    fs::create_directories(FilePath.parent_path());
                }

                FileStream.open(FilePath, std::ios::app);
                if (!FileStream.is_open()) {
                    std::cerr << "Ошибка открытия файла " << FilePath << std::endl;
                }
            } catch (const fs::filesystem_error& ex) {
                std::cerr << "Ошибка работы с файловой системой: " << ex.what() << std::endl;
            }
        }

        TLogger(const TLogger&) = delete;
        TLogger& operator=(const TLogger&) = delete;

    public:
        static TLogger* getInstance(const fs::path& path) {
            std::unique_lock<std::mutex> lock(InstanceMutex);

            if (Instance.find(path) == Instance.end()) {
                Instance[path] = new TLogger(path);
            }
            return Instance[path];
        }

        void log(const std::string& message) {
            std::unique_lock lock(WriteMutex);
            if (FileStream.is_open()) {
                FileStream << message << std::endl;
            }
        }

        static void multiLog(const std::vector<fs::path>& paths, const std::string& message) {
            for (const auto& path : paths) {
                TLogger* logger = getInstance(path);
                if (logger) {
                    logger->log(message);
                }
            }
        }

        static void destroyAll() {
            std::unique_lock<std::mutex> lock(InstanceMutex);

            for (auto& pair : Instance) {
                delete pair.second;
            }
            Instance.clear();
        }

        ~TLogger() {
            if (FileStream.is_open()) {
                FileStream.close();
            }
        }
    };
}
