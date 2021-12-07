cmake_minimum_required(VERSION 3.12)
project(DoodleJump)

set(CMAKE_CXX_STANDARD 14)


SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules")
find_package(SFML 2 COMPONENTS system window graphics)

#file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

SET(SRC main.cpp Game.cpp Game.h Stopwatch.cpp Stopwatch.h Camera.cpp Camera.h Random.cpp Random.h World.cpp World.h BGTile.cpp BGTile.h Bonus.cpp Bonus.h)

INCLUDE_DIRECTORIES(${SFML_INCLUDE_DIR})

add_executable(DoodleJump ${SRC})

TARGET_LINK_LIBRARIES(DoodleJump ${SFML_LIBRARIES})

INSTALL(TARGETS DoodleJump DESTINATION ${PROJECT_SOURCE_DIR}/bin)

if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()