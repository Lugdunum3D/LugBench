#include "json.hpp"

#include <iostream>
#include <fstream>

#include <lug/System/Logger.hpp>
#include <lug/System/Logger/OstreamHandler.hpp>

#include <lug/Window/Window.hpp>
#include <lug/System/Logger.hpp>
#include <lug/System/Logger/OstreamHandler.hpp>
#include <lug/System/Logger/LogCatHandler.hpp>

#include "Application.hpp"

//using json = nlohmann::json;

int main(int, const char*[]) {


int main(int argc, char *argv[]) {
    lug::System::Logger::logger.addHandler(lug::System::makeHandler<lug::System::StdoutHandler>("logcat"));

    Application app;

    if (!app.init(argc, argv)) {
        return 1;
    }

    return app.run() ? 0 : 1;
}