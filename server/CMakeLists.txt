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
    src/CollisionManager.cpp
    src/Mover.cpp
    src/Health.cpp
    components/PlayerCombat.cpp
    components/PlayerSkillType.cpp
    components/EnemyComponent.cpp
    prefabs/Enemy.cpp
    prefabs/AttackManager.cpp
    )
FILE(GLOB COMMON_SOURCES ../_common/src/components/*.cpp ../_common/src/engine/*.cpp ../_common/src/*.cpp)
FILE(GLOB ENEMY_ATTACKS prefabs/enemyAttacks/*.cpp)
FILE(GLOB PLAYER_SKILLS prefabs/playerSkills/*.cpp)

add_executable(server ${SERVER_SOURCES} ${COMMON_SOURCES} ${ENEMY_ATTACKS} ${PLAYER_SKILLS})
set(SERVER_INCLUDE_DIRECTORIES
    "${CMAKE_CURRENT_LIST_DIR}/include"
    "${CMAKE_CURRENT_LIST_DIR}/components"
)
include_directories(server PRIVATE "${SERVER_INCLUDE_DIRECTORIES}")
include_directories(
    .
    "../_common/"
    "../_common/include"
    "./prefabs"
    )

if (CMAKE_SYSTEM_NAME MATCHES WINDOWS)
  set(CMAKE_EXE_LINKER_FLAGS "--static")
  target_link_libraries(server PUBLIC ws2_32)
  target_link_libraries(server PRIVATE pthread)
else()
  #target_link_libraries(${PROJECT_NAME} assimp.5.4.0)
  set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "server.exe")
endif()
