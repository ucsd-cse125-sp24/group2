# the name of the target operating system
set(CMAKE_SYSTEM_NAME OSX)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER   clang)
set(CMAKE_CXX_COMPILER clang++)

set(OPENGL_INCLUDE_DIR "/Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk/System/Library/Frameworks/OpenGL.framework/Headers/")
#set(OPENGL_gl_LIBRARY "/Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk/System/Library/Frameworks/OpenGL.framework/")
set(OPENGL_gl_LIBRARY "/usr/local/Cellar/glew/2.2.0_1/lib")

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH  /Library/Developer/CommandLineTools/SDKs/MacOSX12.3.sdk/System/Library/Frameworks/)

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)