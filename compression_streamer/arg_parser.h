#pragma once

#include <optional>
#include <unordered_map>
#include <string>
#include <sstream>

namespace parser {

    constexpr char DashSymbol = '-';
    constexpr char AssignmentSymbol = '=';
    constexpr char WhiteSpace = ' ';
    constexpr int MinArgumenrLength = 5;
    constexpr int MarkerLength = 2;

    class ArgParser {
        public:
            ArgParser(int argc, char** argv);
            ArgParser(const std::string& text);
            ArgParser(std::stringstream& ss);

            template<typename It>
            ArgParser(It first, It last) {
                std::string buf(first, last);
                std::stringstream ss(buf);
                parse(ss);
            }

            template<typename T>
            std::optional<T> read(std::string key) const {
                if (_data.count(key) == 0)
                    return std::nullopt;
                std::stringstream ss(_data.at(key));
                T out;
                ss >> out;
                return {out};
            }

        private:
            std::unordered_map<std::string, std::string> _data;

            void parse(std::stringstream& ss);
    };

}
