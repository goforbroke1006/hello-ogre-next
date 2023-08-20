cmake_minimum_required(VERSION 3.26)
project(hello-ogre-next)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "DEBUG MODE")
#    add_compile_definitions(_DEBUG=1)
#    add_compile_definitions(OGRE_DEBUG_MODE=1)
endif ()

if (WIN32)
    message(STATUS "TODO: add instructions to support Windows")
endif (WIN32)

if (LINUX)
    set(CMAKE_MODULE_PATH /usr/local/lib/OGRE/cmake)
endif (LINUX)

find_package(OGRE 2.3.1 REQUIRED COMPONENTS Bites RTShaderSystem)
target_include_directories(${PROJECT_NAME} PRIVATE
        ${OGRE_INCLUDE_DIRS}
        #        /usr/local/include/OGRE/Compositor
        #        /usr/local/include/OGRE/Hlms/Common
        #        /usr/local/include/OGRE/Hlms/Pbs
        #        /usr/local/include/OGRE/Hlms/Unlit
)
message(OGRE_LIBRARIES = ${OGRE_LIBRARIES})
target_link_libraries(${PROJECT_NAME}
        ${OGRE_LIBRARIES}
        #        /usr/local/lib/libOgreMain_d.so
        #        /usr/local/lib/libOgreHlmsPbs_d.so
        #        /usr/local/lib/libOgreHlmsUnlit_d.so
)

#configure_file(${CMAKE_SOURCE_DIR}/ogre.cfg ${CMAKE_CURRENT_BINARY_DIR}/ogre.cfg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/plugins.cfg ${CMAKE_CURRENT_BINARY_DIR}/plugins.cfg COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/resources.cfg ${CMAKE_CURRENT_BINARY_DIR}/resources.cfg COPYONLY)
