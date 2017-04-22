#pragma warning(push)  
#pragma warning( disable : 4003)
#include <json/json.hpp>
#pragma warning(pop)

#include <iostream>
#include <fstream>

#include <lug/System/Logger/Logger.hpp>
#if defined(LUG_SYSTEM_ANDROID)
#include <lug/System/Logger/LogCatHandler.hpp>
#else
#include <lug/System/Logger/OstreamHandler.hpp>
#endif
#include <lug/Window/Window.hpp>

#include "Application.hpp"

//using json = nlohmann::json;

//int main(int argc, const char* argv[]) {
int main(int argc, char* argv[]) {
#if defined(LUG_SYSTEM_ANDROID)
    LUG_LOG.addHandler(lug::System::Logger::makeHandler<lug::System::Logger::LogCatHandler>("logcat"));
#else
    LUG_LOG.addHandler(lug::System::Logger::makeHandler<lug::System::Logger::StdoutHandler>("stdout"));
#endif

    Application app;

    if (!app.init(argc, argv)) {
        return 1;
    }

    // query vulkan info
    return 0;

    //    return app.run() ? 0 : 1;
}
