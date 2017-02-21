#pragma once
#include <string>

#define PutFormData = std::map<string,string>

class Router
{
public:
    Router();
    ~Router();

    enum Route
    {
        getAllVulkanInfo,
        getVulkanInfoWithId,
        putVulkanInfo
    };

    //asoni::Handle getHandle(Route route);
    
private:
    std::string baseUrlString;

};

