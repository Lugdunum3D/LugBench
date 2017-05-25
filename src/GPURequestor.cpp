#include "GPURequestor.hpp"




void GPURequestor::putVulkanInfo(nlohmann::json vulkanInfosJson) {
	
	RestClient::Response r = RestClient::put(rout.getUrlString(Router::Route::putVulkanInfo, 0), "text/json", vulkanInfosJson.dump(0));
	std::cout << vulkanInfosJson.dump(4) << std::endl;
	std::cout << r.code << std::endl;

}