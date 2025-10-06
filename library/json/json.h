#pragma once

#include <unordered_map>
#include <string>
#include <initializer_list>

namespace NMafia {
    class TJsonMap {
    private:
        std::unordered_map<std::string, std::string> Data;

    public:
        TJsonMap() = default;

        TJsonMap(const std::initializer_list<std::pair<std::string, std::string>>& init) {
            for (const auto& pair : init) {
                Data[pair.first] = pair.second;
            }
        }

        TJsonMap(const TJsonMap& other) : Data(other.Data) {}

        TJsonMap& operator=(const TJsonMap& other) {
            if (this != &other) {
                Data = other.Data;
            }
            return *this;
        }

        void Add(const std::string& key, const std::string& value) {
            Data[key] = value;
        }

        std::string Get(const std::string& key) const {
            auto it = Data.find(key);
            if (it != Data.end()) {
                return it->second;
            }
            return "";
        }

        bool Contains(const std::string& key) const {
            return Data.count(key) > 0;
        }

        void Remove(const std::string& key) {
            Data.erase(key);
        }

        std::string ToString() const {
            std::string result = "{\n";
            for (const auto& pair : Data) {
                result += "  \"" + pair.first + "\": \"" + pair.second + "\",\n";
            }
            if (!Data.empty()) {
                result.pop_back();
                result.pop_back();
            }
            result += "\n}";
            return result;
        }

        size_t Size() const {
            return Data.size();
        }

        std::string GetOrNull(const std::string& key) const {
            if (Contains(key)) {
                return Get(key);
            } else {
                return "";
            }
        }
    };
}
