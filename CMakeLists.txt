cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 11)

project(DecodeMidi)

set(SRCS
    main.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})

add_subdirectory(midifile EXCLUDE_FROM_ALL)

target_include_directories(${PROJECT_NAME} PRIVATE midifile/include)

target_link_directories(${PROJECT_NAME} PRIVATE midifile)

target_link_libraries(${PROJECT_NAME} midifile)
