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
    ../common/Packet.cpp
    main.cpp
    src/Entity.cpp
    src/Scene.cpp
    )


add_executable(server "${SERVER_SOURCES}")
set(SERVER_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_LIST_DIR}/include"
)
include_directories(server PRIVATE "${SERVER_INCLUDE_DIRECTORIES}")
include_directories(
    "../common/"
    )

if (CMAKE_SYSTEM_NAME MATCHES WINDOWS)
  set(CMAKE_EXE_LINKER_FLAGS "--static")
  target_link_libraries(server PUBLIC ws2_32)
  target_link_libraries(server PRIVATE pthread)
else()
  set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "server.app")
endif()
