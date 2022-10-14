#include "service.h"

#include <cstdlib>
#include <iostream>

int main(int /*argc*/, char* /*argv*/[])
{
  int ret = EXIT_FAILURE;
  std::cout << "Startup..." << std::endl;
  try {
    Service::ConnectionInfo ci {
      "127.0.0.1",
      8888,
      "/Users/matthew.hoggan/Desktop",
      1,
      "/Users/matthew.hoggan/Desktop/server.pem"};
    Service service(ci);
    ret = EXIT_SUCCESS;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    ret = EXIT_FAILURE;
  }
  return ret;
}
