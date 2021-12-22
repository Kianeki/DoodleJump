cmake_minimum_required(VERSION 3.12)
project(DoodleJump)

set(CMAKE_CXX_STANDARD 14)


SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules")
find_package(SFML 2 COMPONENTS system window graphics)

#file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

SET(SRC main.cpp Game.h Game.cpp Camera.cpp Camera.h World.cpp World.h AbstractFactory.cpp AbstractFactory.h EntityModel.cpp EntityModel.h ConcreteFactory.cpp ConcreteFactory.h EntityView.cpp EntityView.h Observer.cpp Observer.h Subject.cpp Subject.h Random.h Random.cpp Stopwatch.h Stopwatch.cpp)

INCLUDE_DIRECTORIES(${SFML_INCLUDE_DIR})

add_executable(DoodleJump ${SRC})

TARGET_LINK_LIBRARIES(DoodleJump ${SFML_LIBRARIES})

INSTALL(TARGETS DoodleJump DESTINATION ${PROJECT_SOURCE_DIR}/bin)

if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()