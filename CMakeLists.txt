cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)

project(OpenCB)


# some magic
add_compile_definitions(SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)  # set logging level before compilation
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)  # https://stackoverflow.com/questions/75387108/how-to-properly-add-a-qrc-file-to-a-qt-6-widget-app-with-cmake-as-building-syst
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
INCLUDE(C:/vcpkg/scripts/buildsystems/vcpkg.cmake)  # so that vcpkg installed packages are seen


# find packages
find_package(Qt6Core REQUIRED)
find_package(Qt6Gui REQUIRED)
find_package(Qt6Widgets REQUIRED)

find_package(OpenCV REQUIRED)

find_package(GTest REQUIRED)

find_package(spdlog REQUIRED)


# include files
set(IP_DIR "${CMAKE_SOURCE_DIR}/ip")
set(UA_DIR "${CMAKE_SOURCE_DIR}/ua")
set(VAR_DIR "${CMAKE_SOURCE_DIR}/var")
set(TEST_DIR "${CMAKE_SOURCE_DIR}/test")
set(LOG_DIR "${CMAKE_SOURCE_DIR}/log")

include_directories(${IP_DIR})
include_directories(${UA_DIR})
include_directories(${VAR_DIR})
include_directories(${TEST_DIR})
include_directories(${LOG_DIR})

file(GLOB_RECURSE SOURCES
    "${IP_DIR}/*.cpp"
    "${IP_DIR}/*.h"
    "${UA_DIR}/*.cpp"
    "${UA_DIR}/*.h"
    "${UA_DIR}/*.qrc"  # tick "Show All Files" in Solution Explorer to see
    "${VAR_DIR}/*.cpp"
    "${VAR_DIR}/*.h"
    "${LOG_DIR}/*.cpp"
    "${LOG_DIR}/*.h"
    "Main.cpp"
)
file(GLOB_RECURSE TEST_SOURCES
    "${TEST_DIR}/*.cpp"
    "${TEST_DIR}/*.h"
    "${VAR_DIR}/*.cpp"  # tests need to see tested functions
    "${VAR_DIR}/*.h"
    "${LOG_DIR}/*.cpp"
    "${LOG_DIR}/*.h"
)
add_executable (OpenCB ${SOURCES})
#add_executable (Tests ${TEST_SOURCES}) # separate executable, depending on gtest


# link libraries
target_link_libraries(OpenCB Qt::Core)
#target_link_libraries(Tests Qt::Core)  # since tests also contain var, dependent on QMetaType
target_link_libraries(OpenCB Qt::Gui)
target_link_libraries(OpenCB Qt::Widgets)

target_link_libraries(OpenCB ${OpenCV_LIBS})

#target_link_libraries(Tests GTest::gtest)

target_link_libraries(OpenCB spdlog::spdlog)
#target_link_libraries(Tests spdlog::spdlog)  # since tests also contain var, dependent on spdlog
