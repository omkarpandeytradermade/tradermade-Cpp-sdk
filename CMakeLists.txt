cmake_minimum_required(VERSION 3.14)
project(TraderMadeSDK LANGUAGES CXX)

add_library(tradermade_sdk STATIC TraderMadeSDK.cpp TraderMadeSDK.h)

target_compile_features(tradermade_sdk PUBLIC cxx_std_14)


target_include_directories(tradermade_sdk PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
