#include "Router.hpp"




Router::Router() : baseUrlString("https://lugdunum-benchmarking-server.herokuapp.com")
{


}

Router::~Router()
{
}


asoni::Handle &Router::getHandle(Router::Route route, int id = -1, PutFormData &formData = nullptr) {
    switch (route)
    {
    case getAllVulkanInfo:
        return Http().get(baseUrlString + "/vulkaninfos");
        break;
    case getVulkanInfoWithId:
        return  Http().get(baseUrlString + "/vulkaninfos/" + std::string(id));
        break;
    case putVulkanInfo:
        return  Http().get(baseUrlString + "/vulkaninfos/" + std::string(id));
        break
    default:
        break;
    }
}