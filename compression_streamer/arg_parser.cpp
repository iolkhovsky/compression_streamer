#include "arg_parser.h"

#include <sstream>
#include <string_view>

namespace parser {

    ArgParser::ArgParser(int argc, char** argv) {
        std::stringstream ss;
        for (int i = 0; i < argc; i++) {
            if (i)
                ss << WhiteSpace;
            ss << argv[i];
        }
        parse(ss);
    }

    ArgParser::ArgParser(const std::string& text) {
        std::stringstream ss(text);
        parse(ss);
    }

    ArgParser::ArgParser(std::stringstream& ss) {
        parse(ss);
    }

    void ArgParser::parse(std::stringstream& ss) {
        std::string buf;
        while(ss >> buf) {
            if ((buf[0] == DashSymbol) && (buf[1] == DashSymbol) && (buf.size() >= MinArgumenrLength)) {
                auto divider_pos = buf.find(AssignmentSymbol);
                if ((divider_pos != std::string::npos) && (divider_pos < buf.size() - 1)) {
                    std::string key(std::next(buf.begin(), MarkerLength), std::next(buf.begin(), divider_pos));
                    std::string val(std::next(buf.begin(), divider_pos + 1), buf.end());
                    _data[key] = std::move(val);
                }
            }
        }
    }

}
