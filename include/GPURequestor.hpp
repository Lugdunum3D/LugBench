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

using JSONResponse = std::tuple<int, nlohmann::json>;
using ResponseCode = int;

class GPURequestor {
public:
	GPURequestor() = default;
	
	GPURequestor(const GPURequestor&) = delete;

	GPURequestor& operator=(const GPURequestor&) = delete;
	GPURequestor& operator=(const GPURequestor&&) = delete;

	~GPURequestor() = default;
	
	ResponseCode putVulkanInfo(nlohmann::json vulkanInfosJson);
	JSONResponse getAllVulkanInfo();
	JSONResponse getVulkanInfoWithID(uint16_t id);
	
private:
	Router rout;

private:
	JSONResponse getRequestResponse(std::string request);
};