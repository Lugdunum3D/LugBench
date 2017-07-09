#include <Application.hpp>

#include <ctime>
#include <sstream>

#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

#include <APIClient/GPU.hpp>
#include <APIClient/Router.hpp>
#include <GPUInfoProvider.hpp>

#include "AState.hpp"
#include "MenuState.hpp"
#include "BenchmarkingState.hpp"

#include <lug/Graphics/Light/Directional.hpp>
#include <lug/Graphics/Scene/ModelInstance.hpp>

// TODO: Remove this when the ResourceManager is done
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

#include <lug/System/Time.hpp>

Application::Application() : lug::Core::Application::Application{{"hello", {0, 1, 0}}} {
    std::srand((uint32_t)std::time(0));
    getRenderWindowInfo().windowInitInfo.title = "LugBench";

    getRenderWindowInfo().renderViewsInitInfo.push_back({
        lug::Graphics::Render::Technique::Type::Forward,    // renderTechniqueType
        {                                                   // viewport
            {                                               // offset
                0.0f,                                       // x
                0.0f                                        // y
            },

            {                                               // extent
                1.0f,                                       // width
                1.0f                                        // height
            },

            0.0f,                                           // minDepth
            1.0f                                            // maxDepth
        },
        {                                                   // scissor
            {                                               // offset
                0.0f,                                       // x
                0.0f                                        // y
            },
            {                                               // extent
                1.0f,                                       // width
                1.0f                                        // height
            }
        },
        nullptr                                             // camera
    });

}

Application::~Application() {
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();
}

bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::beginInit(argc, argv)) {
        return false;
    }

	lug::Graphics::Renderer* renderer = _graphics.getRenderer();

	lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);



	for (auto& choosedDevice : vkRender->getPhysicalDeviceInfos()) {

		if (!initDevice(&choosedDevice)) {

			LUG_LOG.warn("Can't initialize the engine for the device {}", choosedDevice.properties.deviceName);

		}
	}

    // // Create the scene
    // _scene = _graphics.createScene();

    // // Add directional light to scene
    // {
    //     std::unique_ptr<lug::Graphics::Light::Light> light = _scene->createLight("light", lug::Graphics::Light::Light::Type::Directional);

    //     // Set the diffuse color to white and the position
    //     light->setDiffuse({1.0f, 1.0f, 1.0f});
    //     static_cast<lug::Graphics::Light::Directional*>(light.get())->setDirection({0.0f, -4.0f, 5.0f});

    //     _scene->getRoot()->createSceneNode("light node", std::move(light));
    // }

    // Create camera
    _camera = _graphics.createCamera("camera");
//    camera->setScene(_scene.get());

    // // Add camera to scene
    // {
    //     std::unique_ptr<lug::Graphics::Scene::MovableCamera> movableCamera = _scene->createMovableCamera("movable camera", camera.get());

    //     std::unique_ptr<lug::Graphics::Scene::Node> movableCameraNode = _scene->createSceneNode("movable camera node");

    //     movableCameraNode->attachMovableObject(std::move(movableCamera));

    //     _scene->getRoot()->attachChild(std::move(movableCameraNode));
    // }

    // // Attach cameras to RenderView
    // {
    //     auto& renderViews = _graphics.getRenderer()->getWindow()->getRenderViews();

    //     LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

    //     renderViews[0]->attachCamera(std::move(camera));
    // }
	
    std::shared_ptr<AState> menuState;

    menuState = std::make_shared<MenuState>(*this);
    pushState(menuState);

    return true;
}

bool Application::initDevice(lug::Graphics::Vulkan::PhysicalDeviceInfo* choosedDevice) {
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);

    vkRender->getPreferences().device = choosedDevice;

    if (!lug::Core::Application::finishInit()) {
        return false;
    }

    const lug::Graphics::Vulkan::PhysicalDeviceInfo *physicalDeviceInfo = vkRender->getPhysicalDeviceInfo();
    if (physicalDeviceInfo == NULL) {
        return false;
    }
    const nlohmann::json json = GPUInfoProvider::get(*physicalDeviceInfo);
    #if !defined(LUG_SYSTEM_ANDROID) // TODO(Yoann) FMT has memory corruption on Android with big string
        //LUG_LOG.info("{}", json.dump());
    #endif
    //APIClient::GPU::put(json);
    return (true);
}

void Application::onEvent(const lug::Window::Event& event) {

    if (_states.empty()) {
        return;
    }

    std::shared_ptr<AState> tmpState = _states.top(); // Needed to prevent fault if popping a state

    tmpState->onEvent(event);
}

void Application::onFrame(const lug::System::Time& elapsedTime) {

	if (_states.empty()) {
		return;
	}

    std::shared_ptr<AState> tmpState = _states.top(); // Needed to prevent fault if popping a state

    tmpState->onFrame(elapsedTime);
}

bool Application::haveState() {
    return (!_states.empty());
}

bool Application::popState() {
    if (!_states.top()->onPop()) {
        return (false);
    }
    _states.pop();
    if (!_states.empty()) {
        _states.top()->onPlay();
    }
    // } else {
    //     close();
    // }
    return (true);
}

bool Application::pushState(std::shared_ptr<AState> &state) {
    if (!_states.empty()) {
        _states.top()->onPause();
    }
    _states.push(state);
    if (!_states.top()->onPush()) {
        return (false);
    }
    return (true);
}

bool Application::popAndPushState(std::shared_ptr<AState> &state) {
    _states.pop();
    _states.push(state);
    return (true);
}
