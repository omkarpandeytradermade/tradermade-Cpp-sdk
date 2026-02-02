cmake_minimum_required(VERSION 3.14)
project(TraderMadeSDK LANGUAGES CXX)


include(FetchContent)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)


add_library(tradermade_sdk STATIC TraderMadeSDK.cpp TraderMadeSDK.h)

target_compile_features(tradermade_sdk PUBLIC cxx_std_14)


target_link_libraries(tradermade_sdk PUBLIC nlohmann_json::nlohmann_json)

target_include_directories(tradermade_sdk PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)