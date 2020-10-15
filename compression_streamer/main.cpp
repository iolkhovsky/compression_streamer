#include "configurator.h"
#include "client.h"
#include "server.h"

#include <boost/stacktrace.hpp>
#include <boost/filesystem.hpp>

#include <iostream>

int main(int argc, char** argv) {
    try {
        streamer::Configurator config(argc, argv);
        std::cout << config;
        if (config.GetMode() == streamer::Configurator::GlobalModes::server)
            run_server(config);
        else
            run_client(config);
    } catch (std::exception& e) {
        std::cout << "Unexpected exception: " << e.what() << std::endl;
        boost::stacktrace::safe_dump_to("./backtrace.dump");
        std::cout << boost::stacktrace::stacktrace();
        if (boost::filesystem::exists("./backtrace.dump")) {
            std::ifstream ifs("./backtrace.dump");

            boost::stacktrace::stacktrace st = boost::stacktrace::stacktrace::from_dump(ifs);
            std::cout << "Previous run crashed:\n" << st << std::endl;

            ifs.close();
            boost::filesystem::remove("./backtrace.dump");
        }
    } catch (...) {
        boost::stacktrace::safe_dump_to("./backtrace.dump");
        std::cout << boost::stacktrace::stacktrace();
        if (boost::filesystem::exists("./backtrace.dump")) {
            std::ifstream ifs("./backtrace.dump");

            boost::stacktrace::stacktrace st = boost::stacktrace::stacktrace::from_dump(ifs);
            std::cout << "Previous run crashed:\n" << st << std::endl;

            ifs.close();
            boost::filesystem::remove("./backtrace.dump");
        }
    }
    return 0;
}
