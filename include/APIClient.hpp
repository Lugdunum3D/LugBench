#pragma once
#include <cpprest\http_client.h>
#include <cpprest\json.h>

#include <lug/Graphics/Vulkan/Vulkan.hpp>

class APIClient : web::http::client::http_client
{
public:
	APIClient();
	~APIClient();

    pplx::task<web::http::http_response> GETDevices(const std::string id);
    pplx::task<web::http::http_response> POSTDevices(const lug::Graphics::Vulkan::PhysicalDeviceInfo& physicalDeviceInfo);

    pplx::task<web::http::http_response> GETScenarios(const std::string id);

    pplx::task<web::http::http_response> GETScores(const std::string id);
    pplx::task<web::http::http_response> POSTScore(const std::map < std::string, std::string> score);

    //TODO (2): creer une methode private Get / post
};

