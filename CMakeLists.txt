cmake_minimum_required(VERSION 3.0)

project(DecodeMidi)

set(SRCS
    main.cpp
)

add_subdirectory(midifile)

add_executable(decode ${SRCS})

target_link_libraries(decode midifile)
