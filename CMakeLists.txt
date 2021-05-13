cmake_minimum_required(VERSION 3.15)
project(game)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCE_LIST
    "${CMAKE_CURRENT_LIST_DIR}/src/*.c")

file(GLOB_RECURSE HEADER_LIST
    "${CMAKE_CURRENT_LIST_DIR}/include/*.h")

set (source "${CMAKE_SOURCE_DIR}/resources")
set (destination "${CMAKE_CURRENT_BINARY_DIR}/resources")
set(GLAD_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/externals/glad/include/")

#cglm math library for opengl
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/externals/cglm/ EXCLUDE_FROM_ALL)
#GLAD
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/externals/glad)
add_executable(${PROJECT_NAME} ${HEADER_LIST} ${SOURCE_LIST})

message("SYSTEM IS:" "${CMAKE_SYSTEM_NAME}")


add_custom_command(
    TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
    #COMMAND ${CMAKE_COMMAND} -E copy ${source} ${destination}
    DEPENDS ${destination}
    COMMENT "symbolic link resources folder from ${source} => ${destination}")

find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${SDL2_INCLUDE_DIR}
    #${SDL2_IMAGE_INCLUDE_DIRS}
    )

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)


target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} glad ${CMAKE_DL_LIBS} cglm_headers m)
