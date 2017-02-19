#include <ctime>
#include <sstream>
#include "Application.hpp"

#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

Application::Application() : lug::Core::Application::Application{{"hello", {0, 1, 0}}} {
    std::srand((uint32_t)std::time(0));
    getRenderWindowInfo().windowInitInfo.title = "LugBench";

    getRenderWindowInfo().renderViewsInitInfo.push_back({
        lug::Graphics::RenderTechnique::Type::Forward,  // renderTechniqueType
        {                                               // viewport
            {                                           // offset
                0.0f,                                   // x
                0.0f                                    // y
            },

            {                                           // extent
                0.5f,                                   // width
                1.0f                                    // height
            },

            0.0f,                                       // minDepth
            1.0f                                        // maxDepth
        },
        {                                               // scissor
            {                                           // offset
                0.0f,                                   // x
                0.0f                                    // y
            },
            {                                           // extent
                1.0f,                                   // width
                1.0f                                    // height
            }
        },
        nullptr                                         // camera
    });

    getRenderWindowInfo().renderViewsInitInfo.push_back({
        lug::Graphics::RenderTechnique::Type::Forward,  // renderTechniqueType
        {                                               // viewport
            {                                           // offset
                0.5f,                                   // x
                0.0f                                    // y
            },

            {                                           // extent
                0.5f,                                   // width
                1.0f                                    // height
            },

            0.0f,                                       // minDepth
            1.0f                                        // maxDepth
        },
        {                                               // scissor
            {                                           // offset
                0.0f,                                   // x
                0.0f                                    // y
            },
            {                                           // extent
                1.0f,                                   // width
                1.0f                                    // height
            }
        },
        nullptr                                         // camera
    });

    getGraphicsInfo().rendererInitInfo.useDiscreteGPU = false;
}

Application::~Application() {
    // TODO: Remove this when the ResourceManager is done
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    for (auto& queue: vkRender->getQueues()) {
        queue.waitIdle();
    }
}

bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::init(argc, argv)) {
        return false;
    }
    _scene = _graphics.createScene();
    return true;
}

void Application::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::EventType::CLOSE) {
        close();
    }
}

void Application::onFrame(const lug::System::Time& elapsedTime) {
    (void)elapsedTime;
    //_cubeNode->rotate(0.5f * elapsedTime, {0.0f, 1.0f, 0.0f});
}
