cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(server)

# Server config
set(SERVER_SOURCES 
    src/Client.cpp
    src/NetworkManager.cpp
    src/Player.cpp
    src/Server.cpp
    main.cpp
    )

add_executable(server "${SERVER_SOURCES}")
set(SERVER_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_LIST_DIR}/include"
)
include_directories(server PUBLIC "${SERVER_INCLUDE_DIRECTORIES}")
target_link_libraries(server ws2_32)
