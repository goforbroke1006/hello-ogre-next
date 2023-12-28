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
        utils.h
        Hlms.h)

if (WIN32)
    message(STATUS "TODO: add instructions to support Windows")
endif (WIN32)

if (LINUX)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /usr/local/lib/OGRE/cmake)
endif (LINUX)

find_package(OGRE 2.3.3 REQUIRED COMPONENTS Bites RTShaderSystem)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${OGRE_INCLUDE_DIRS}
        ${OGRE_INCLUDE_DIRS}/Hlms
        ${OGRE_INCLUDE_DIRS}/Hlms/Common
        ${OGRE_HlmsPbs_INCLUDE_DIRS}
        ${OGRE_HlmsUnlit_INCLUDE_DIRS}
        "${OGRE_HlmsUnlit_INCLUDE_DIRS}/../Common"
)

target_link_libraries(${PROJECT_NAME}
        ${OGRE_LIBRARIES}
        ${OGRE_HlmsPbs_LIBRARIES}
        ${OGRE_HlmsUnlit_LIBRARIES} OgreOverlay)

configure_file(${CMAKE_SOURCE_DIR}/third_party/ogre-next/build/Debug/bin/plugins_d.cfg ${CMAKE_CURRENT_BINARY_DIR}/plugins_d.cfg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/resources_d.cfg ${CMAKE_CURRENT_BINARY_DIR}/resources_d.cfg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/resources2.cfg ${CMAKE_CURRENT_BINARY_DIR}/resources2.cfg COPYONLY)

file(COPY ./Media DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ./third_party/ogre-next/Samples/Media/Hlms DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Media)
file(COPY ./third_party/ogre-next/Samples/Media/packs DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Media)
