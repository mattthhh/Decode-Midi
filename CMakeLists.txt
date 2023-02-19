cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 11)

project(DecodeMidi)

set(SRCS
    main.cpp
    display.cpp
)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

add_executable(${PROJECT_NAME} ${SRCS})

add_subdirectory(midifile)

target_include_directories(${PROJECT_NAME} PRIVATE midifile/include)

target_link_directories(${PROJECT_NAME} PRIVATE midifile)

target_link_libraries(${PROJECT_NAME} midifile)
target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES})
