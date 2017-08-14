#include <lug/Config.hpp>

#include <json/json.hpp>

#include <lug/System/Logger/Logger.hpp>
#if defined(LUG_SYSTEM_ANDROID)
    #include <lug/System/Logger/LogCatHandler.hpp>
#else
    #include <lug/System/Logger/OstreamHandler.hpp>
    #include <lug/Window/Window.hpp>
#endif

#include "Application.hpp"

int main(int argc, char* argv[]) {
#if defined(LUG_SYSTEM_ANDROID)
    LUG_LOG.addHandler(lug::System::Logger::makeHandler<lug::System::Logger::LogCatHandler>("logcat"));
#else
    LUG_LOG.addHandler(lug::System::Logger::makeHandler<lug::System::Logger::StdoutHandler>("stdout"));
#endif

    LugBench::Application app;

    if (!app.init(argc, argv)) {
        return 1;
    }

    return app.run() ? 0 : 1;
}
