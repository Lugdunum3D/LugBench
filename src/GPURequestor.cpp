#include "GPURequestor.hpp"




int GPURequestor::putVulkanInfo(nlohmann::json vulkanInfosJson) {
	
	RestClient::Response r = RestClient::put(rout.getUrlString(Router::Route::putVulkanInfo, 0), "text/json", vulkanInfosJson.dump(0));
	return r.code;
}

JSONResponse GPURequestor::getAllVulkanInfo()
{
	return	getRequestResponse(rout.getUrlString(Router::Route::getAllVulkanInfo, 0));

}

JSONResponse GPURequestor::getVulkanInfoWithID(int id)
{
	return	getRequestResponse(rout.getUrlString(Router::Route::getVulkanInfoWithId, id));
}

JSONResponse GPURequestor::getRequestResponse(std::string request)
{
	nlohmann::json json;
	RestClient::Response r = RestClient::get(request);

	if (r.code >= 300 && r.code < 400)
	{
		json = nlohmann::json(r.body);
	}
	return (r.code, json);
}