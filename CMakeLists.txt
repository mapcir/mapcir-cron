cmake_minimum_required(VERSION 3.20.2 FATAL_ERROR)

project(mapcir-cron LANGUAGES CXX)

## cmake -DCMAKE_BUILD_TYPE=Release
# set(CMAKE_BUILD_TYPE Release)

# Use C++11
set(CMAKE_CXX_STANDARD 11)
# Require (at least) it
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Don't use e.g. GNU extension (like -std=gnu++11) for portability
set(CMAKE_CXX_EXTENSIONS OFF)

# nlohmann_json
set(JSON_BuildTests OFF CACHE INTERNAL "")
if(NOT TARGET nlohmann_json)
  find_package(nlohmann_json CONFIG REQUIRED)
endif()

# libs
file(GLOB HELPER "src/helpers/*.cc")

add_executable(${PROJECT_NAME} src/main.cc ${HELPER})
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)