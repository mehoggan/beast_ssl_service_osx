cmake_minimum_required(VERSION 3.0.2)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

set(SRC
  ./endpoints.cxx
  ./https_session.cxx
  ./main.cxx
  ./service.cxx
  ./service_helpers.cxx)

add_executable(https_service ${SRC})

target_link_libraries(https_service
  boost_filesystem
  boost_system
  boost_thread
  ${OPENSSL_LIB}
  crypto
  pthread
)
