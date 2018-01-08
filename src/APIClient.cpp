#include "APIClient.hpp"
#include <cpprest\uri_builder.h>

// TODO (1): recuperer l'url depuis le cmake 
APIClient::APIClient() : web::http::client::http_client(U("http://lugbench-api-dev.herokuapp.com"))
{

}


APIClient::~APIClient()
{
}

pplx::task<web::http::http_response> APIClient::GETDevices(const std::string id)
{
    web::uri_builder builder(U("/devices"));

    return this->request(web::http::methods::GET, builder.to_string());
}
