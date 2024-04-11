cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(client)  

# Client config
set(CLIENT_SOURCES
    src/client.cpp
    main.cpp
    )
add_executable(client "${CLIENT_SOURCES}")
set(CLIENT_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_LIST_DIR}/include"
)
target_include_directories(client PRIVATE "${CLIENT_INCLUDE_DIRECTORIES}")
target_link_libraries(client ws2_32)