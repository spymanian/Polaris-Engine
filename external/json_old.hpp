// Single-header JSON library for C++ (nlohmann/json v3.11.3)
// Download from: https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
// This is a placeholder - you'll need to download the actual header from:
// https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp

#pragma once
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <sstream>

// MINIMAL JSON PARSER (for demo - replace with full nlohmann/json for production)
namespace nlohmann {
    class json {
        enum Type { Null, Number, String, Bool, Object, Array };
        Type type_ = Null;
        double number_ = 0;
        std::string string_;
        bool bool_ = false;
        std::map<std::string, json> object_;
        std::vector<json> array_;

    public:
        json() = default;
        json(double v) : type_(Number), number_(v) {}
        json(int v) : type_(Number), number_(v) {}
        json(bool v) : type_(Bool), bool_(v) {}
        json(const char* v) : type_(String), string_(v) {}
        json(const std::string& v) : type_(String), string_(v) {}

        static json parse(std::istream& is) {
            json result;
            // Simplified: just create empty object for now
            result.type_ = Object;
            return result;
        }

        json& operator[](const std::string& key) {
            if (type_ != Object) type_ = Object;
            return object_[key];
        }

        const json& operator[](const std::string& key) const {
            static json null_json;
            auto it = object_.find(key);
            return it != object_.end() ? it->second : null_json;
        }

        template<typename T>
        T get() const {
            if constexpr (std::is_same_v<T, double>) return number_;
            else if constexpr (std::is_same_v<T, int>) return static_cast<int>(number_);
            else if constexpr (std::is_same_v<T, bool>) return bool_;
            else if constexpr (std::is_same_v<T, std::string>) return string_;
            return T{};
        }

        bool contains(const std::string& key) const {
            return object_.find(key) != object_.end();
        }

        bool is_null() const { return type_ == Null; }
    };
}
