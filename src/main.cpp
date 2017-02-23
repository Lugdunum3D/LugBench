#include "json.hpp"

#include <iostream>
#include <fstream>

#include <lug/System/Logger/Logger.hpp>
#include <lug/System/Logger/OstreamHandler.hpp>

#include <lug/Window/Window.hpp>

#include "Application.hpp"

//using json = nlohmann::json;

//int main(int argc, const char* argv[]) {
int main(int argc, char* argv[]) {
    LUG_LOG.addHandler(lug::System::Logger::makeHandler<lug::System::Logger::StdoutHandler>("stdout"));

    Application app;

    if (!app.init(argc, argv)) {
        return 1;
    }

    // query vulkan info
    return 0;

    //    return app.run() ? 0 : 1;
}
