#pragma once

#include <filesystem>
#include <fstream>
#include <string>
#include <map>
#include <mutex>
#include <iostream>


namespace NMafia {

    namespace fs = std::filesystem;

    class Logger {
    private:
        static std::map<fs::path, Logger*> instances;
        static std::mutex mutex;
        std::ofstream fileStream;
        fs::path filePath;

        Logger(const fs::path& path) : filePath(path) {
            try {
                if (!fs::exists(filePath.parent_path())) {
                    fs::create_directories(filePath.parent_path());
                }

                fileStream.open(filePath, std::ios::app);
                if (!fileStream.is_open()) {
                    std::cerr << "Ошибка открытия файла " << filePath << std::endl;
                }
            } catch (const fs::filesystem_error& ex) {
                std::cerr << "Ошибка работы с файловой системой: " << ex.what() << std::endl;
            }
        }

        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

    public:
        static Logger* getInstance(const fs::path& path) {
            std::lock_guard<std::mutex> lock(mutex);

            if (instances.find(path) == instances.end()) {
                instances[path] = new Logger(path);
            }
            return instances[path];
        }

        void log(const std::string& message) {
            if (fileStream.is_open()) {
                fileStream << message << std::endl;
            }
        }

        static void destroyAll() {
            std::lock_guard<std::mutex> lock(mutex);

            for (auto& pair : instances) {
                delete pair.second;
            }
            instances.clear();
        }

        ~Logger() {
            if (fileStream.is_open()) {
                fileStream.close();
            }
        }
    };

    std::map<fs::path, Logger*> Logger::instances = {};
    std::mutex Logger::mutex;
}
