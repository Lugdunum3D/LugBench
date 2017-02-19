#include "json.hpp"
#include <iostream>
#include <fstream>
#include <lug/Window/Window.hpp>
#include <lug/System/Logger.hpp>
#include <lug/System/Logger/OstreamHandler.hpp>
#include <lug/System/Logger/LogCatHandler.hpp>

using json = nlohmann::json;

int main(int, const char*[]) {

    lug::System::Logger::logger.addHandler(lug::System::makeHandler<lug::System::LogCatHandler>("logcat"));
  auto window = lug::Window::Window::create({800, 600, "Default Window", \
					    lug::Window::Style::Default});


  if (!window) {
    return 1;
  }
  // While window is open execute the following
  while (window->isOpen()) {
    lug::Window::Event event;
    while (window->pollEvent(event)) {
      if (event.type == lug::Window::EventType::CLOSE) {
	window->close();
      }
    }
  }
  
  // create an empty structure (null)
  json j;

  // add a number that is stored as double (note the implicit conversion of j to an object)
  j["pi"] = 3.141;

  // add a Boolean that is stored as bool
  j["happy"] = true;

  // add a string that is stored as std::string
  j["name"] = "Niels";

  // add another null object by passing nullptr
  j["nothing"] = nullptr;

  // add an object inside the object
  j["answer"]["everything"] = 42;

  // add an array that is stored as std::vector (using an initializer list)
  j["list"] = { 1, 0, 2 }; 

  // add another object (using an initializer list of pairs)
  j["object"] = { {"currency", "USD"}, {"value", 42.99} };

  // instead, you could also write (which looks very similar to the JSON above)
  json j2 = {
    {"pi", 3.141},
    {"happy", true},
    {"name", "Niels"},
    {"nothing", nullptr},
    {"answer", {
	{"everything", 42}
      }},
    {"list", {1, 0, 2}},
    {"object", {
	{"currency", "USD"},
	{"value", 42.99}
      }}
  };


    lug::System::Logger::logger.info(j.dump(4));


  // write prettified JSON to another file
  std::ofstream o("pretty.json");
  o << std::setw(4) << j << std::endl;
}
