cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(server)

# Server config
set(SERVER_SOURCES 
    src/client.cpp
    src/network_manager.cpp
    src/player.cpp
    src/server.cpp
    main.cpp
    )
add_executable(server "${SERVER_SOURCES}")
set(SERVER_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_LIST_DIR}/include"
)
target_include_directories(server PRIVATE "${SERVER_INCLUDE_DIRECTORIES}")