#include "APIClient.hpp"


// TODO (1): recuperer l'url depuis le cmake 
APIClient::APIClient() : web::http::client::http_client(U("http://lugbench-api-dev.herokuapp.com"))
{

}


APIClient::~APIClient()
{
}
