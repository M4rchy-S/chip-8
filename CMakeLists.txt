cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME chip8project)

set(PROJECT_SRC
        src/ChipEmulator.cpp
        src/CHIP8.cpp
)
set(PROJECT_HEADERS
        headers/CHIP8.h
)

find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
find_package(nfd CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${PROJECT_SRC} ${PROJECT_HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)
target_link_libraries(${PROJECT_NAME} PRIVATE nfd::nfd)

configure_file(roms/IBM.ch8 ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)