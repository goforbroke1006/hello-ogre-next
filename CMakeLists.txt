cmake_minimum_required(VERSION 3.26)
project(hello-ogre-next)

set(CMAKE_CXX_STANDARD 17)

message(CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE})
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    add_compile_definitions(OGRE_DEBUG_MODE)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

add_executable(${PROJECT_NAME} main.cpp
        WindowEventListener.h
        utils.h)

if (WIN32)
    message(STATUS "TODO: add instructions to support Windows")
endif (WIN32)

if (LINUX)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/OGRE/cmake)
endif (LINUX)



find_package(OGRE 2.3.3 REQUIRED COMPONENTS Bites RTShaderSystem)
target_include_directories(${PROJECT_NAME} PRIVATE ${OGRE_INCLUDE_DIRS})
message(OGRE_LIBRARIES = ${OGRE_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OGRE_LIBRARIES})

##configure_file(${CMAKE_SOURCE_DIR}/ogre.cfg ${CMAKE_CURRENT_BINARY_DIR}/ogre.cfg COPYONLY)
#configure_file(${CMAKE_SOURCE_DIR}/plugins.cfg ${CMAKE_CURRENT_BINARY_DIR}/plugins.cfg COPYONLY)
#configure_file(${CMAKE_SOURCE_DIR}/resources.cfg ${CMAKE_CURRENT_BINARY_DIR}/resources.cfg COPYONLY)
#file(COPY ./Media DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

configure_file(${CMAKE_SOURCE_DIR}/ogre.cfg ${CMAKE_CURRENT_BINARY_DIR}/ogre.cfg COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/third_party/ogre-next/build/Debug/bin/plugins_d.cfg ${CMAKE_CURRENT_BINARY_DIR}/plugins_d.cfg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/third_party/ogre-next/build/Debug/bin/resources2.cfg ${CMAKE_CURRENT_BINARY_DIR}/resources2.cfg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/third_party/ogre-next/build/Debug/bin/resources_d.cfg ${CMAKE_CURRENT_BINARY_DIR}/resources_d.cfg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/third_party/ogre-next/build/Debug/bin/samples_d.cfg ${CMAKE_CURRENT_BINARY_DIR}/samples_d.cfg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/third_party/ogre-next/build/Debug/bin/tests_d.cfg ${CMAKE_CURRENT_BINARY_DIR}/tests_d.cfg COPYONLY)
file(COPY ./Media DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
