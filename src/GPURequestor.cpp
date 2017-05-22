#include "GPURequestor.hpp"




void GPURequestor::putVulkanInfo(nlohmann::json vulkanInfosJson) {
	Router rout;
	RestClient::Response r = RestClient::post(rout.getUrlString(Router::Route::putVulkanInfo, 0), "text/json", vulkanInfosJson);
	
	std::cout << r.code << std::endl;

}