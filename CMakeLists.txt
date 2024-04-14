cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
    # 125-Starter
    src/Mover.cpp
)

# Add header files
set(
    HEADERS
    include/core.h
    include/Camera.h
    include/Cube.h
    include/Shader.h
    include/Tokenizer.h
    include/Window.h
    include/Client.h
    # 125-Starter
    include/Mover.h
)

# Require GL

# Add include directories
include_directories(
    include
)

# Add library directories
link_directories(
    lib
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
if (CMAKE_SYSTEM_NAME MATCHES OSX)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew glfw)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "menv.app")
else()
    set(CMAKE_EXE_LINKER_FLAGS "--static")
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32 glfw3 opengl32 gdi32)
    target_link_libraries(${PROJECT_NAME} pthread ws2_32)
endif()

# Move assets to .exe
add_custom_target(CopyShaders ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/shaders"
	"${CMAKE_BINARY_DIR}/shaders"
)
add_dependencies(menv CopyShaders)

# Server configuration
add_subdirectory(server)