cmake_minimum_required(VERSION 3.15)
project(glad)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -std=c++11 -fPIC")
add_library(${PROJECT_NAME} include/glad/glad.h src/glad.c)
#target_compile_definitions(glad PUBLIC GLAD_GLAPI_EXPORT PRIVATE GLAD_GLAPI_EXPORT_BUILD)
target_include_directories(glad PUBLIC include/)
# adding -fPIC for shared
set_property(TARGET glad PROPERTY POSITION_INDEPENDENT_CODE ON)
