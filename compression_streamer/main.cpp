#include "configurator.h"
#include "client.h"
#include "server.h"

#include <iostream>

int main(int argc, char** argv) {
    streamer::Configurator config(argc, argv);
    std::cout << config;
    if (config.GetMode() == streamer::Configurator::GlobalModes::server)
        run_server(config);
    else
        run_client(config);
    return 0;
}
