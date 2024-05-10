cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -L/mnt/c/Users/Shane/Desktop/cse125/client/lib/fmod.dll")

project(menv)

# Add source files
set(
    SOURCES
    main.cpp
    src/Camera.cpp
    src/Cube.cpp
    src/Shader.cpp
    src/Tokenizer.cpp
    src/Window.cpp
    src/Client.cpp
    src/AudioManager.cpp
    src/Entity.cpp

    # graphics
    src/Mesh.cpp
    src/AnimationClip.cpp
    src/AnimationPlayer.cpp
    src/InputManager.cpp
    src/Bone.cpp
    src/components/Model.cpp
    src/components/RendererComponent.cpp

    # 125-Starter
    src/Mover.cpp
    src/GameManager.cpp
    src/Player.cpp
)
FILE(GLOB COMMON_SOURCES ../_common/src/**/*.cpp ../_common/src/*.cpp)


# Require GL

# Add include directories
include_directories(
    include
    ../_common
    ../_common/include
    ../_common/fmod/inc
)
# Add library directories
link_directories(
    lib
    ../_common/fmod/lib
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${COMMON_SOURCES} ${HEADERS})

# Link libraries
if (CMAKE_SYSTEM_NAME MATCHES OSX)
    target_link_libraries(${PROJECT_NAME} assimp.5)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew glfw)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
    target_link_libraries(${PROJECT_NAME} fmod)
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "menv.app")
else()
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
    target_link_libraries(${PROJECT_NAME} assimp zlibstatic)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32 glfw3 opengl32 gdi32)
    target_link_libraries(${PROJECT_NAME} pthread ws2_32)
    target_link_libraries(${PROJECT_NAME} fmod)
endif()

add_custom_target(CopyShaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/shaders"
    "${CMAKE_BINARY_DIR}/shaders"
)

add_dependencies(menv CopyShaders)