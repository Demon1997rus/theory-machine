cmake_minimum_required(VERSION 2.8.12)

SET(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "Release" FORCE)

project(theory-machine)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
