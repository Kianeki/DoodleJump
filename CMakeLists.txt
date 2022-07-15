cmake_minimum_required(VERSION 3.12)
project(DoodleJump)

set(CMAKE_CXX_STANDARD 14)


SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules")
find_package(SFML 2 COMPONENTS system window graphics)

ADD_LIBRARY(logic STATIC AbstractFactory.h AbstractFactory.cpp Camera.cpp Camera.h EntityModel.h EntityModel.cpp Observer.h Observer.cpp Random.cpp Random.h Score.h Score.cpp Stopwatch.cpp Stopwatch.h Subject.cpp Subject.h World.cpp World.h)
#SET(SRC main.cpp Game.h Game.cpp Camera.cpp Camera.h World.cpp World.h AbstractFactory.cpp AbstractFactory.h EntityModel.cpp EntityModel.h ConcreteFactory.cpp ConcreteFactory.h EntityView.cpp EntityView.h Observer.cpp Observer.h Subject.cpp Subject.h Random.h Random.cpp Stopwatch.h Stopwatch.cpp Score.cpp Score.h ScoreView.cpp ScoreView.h)
SET(SRC main.cpp Game.h Game.cpp ConcreteFactory.cpp ConcreteFactory.h EntityView.cpp EntityView.h ScoreView.cpp ScoreView.h HPView.cpp HPView.h)

INCLUDE_DIRECTORIES(${SFML_INCLUDE_DIR})

add_executable(DoodleJump ${SRC})

TARGET_LINK_LIBRARIES(DoodleJump logic)
TARGET_LINK_LIBRARIES(DoodleJump ${SFML_LIBRARIES})

INSTALL(TARGETS DoodleJump DESTINATION ${PROJECT_SOURCE_DIR}/bin)

if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()