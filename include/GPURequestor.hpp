#include "Router.hpp"
#include "restclient-cpp/restclient.h"
#include "restclient-cpp/connection.h"

#if defined(LUG_SYSTEM_WINDOWS)
    #pragma warning(push)
    #pragma warning(disable : 4003)
#endif
#include <json/json.hpp>
#if defined(LUG_SYSTEM_WINDOWS)
    #pragma warning(pop)
#endif


class GPURequestor {
public:
	GPURequestor() = default;
	
	GPURequestor(const GPURequestor&) = delete;
	GPURequestor(GPURequestor&&) = delete;

	GPURequestor& operator=(const GPURequestor&) = delete;
	GPURequestor& operator=(const GPURequestor&&) = delete;

	~GPURequestor() = default;
	
	void putVulkanInfo(nlohmann::json vulkanInfosJson);

private:
	RestClient::Connection baseConnection;
};