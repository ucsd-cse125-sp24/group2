# Getting Started

## Windows Instructions

### Windows Environment Setup

The following environment was tested on multiple Windows environments with Ubuntu 22.04.4 LTS on WSL.

1. Install [WSL 2](https://learn.microsoft.com/en-us/windows/wsl/install)
2. Update your package manager with `sudo apt-get update && sudo apt-get upgrade`
3. Install _MinGW_ with `sudo apt-get install mingw-w64`
4. Install _make_ with `sudo apt-get install make`
5. Install _cmake_ with `sudo apt-get install cmake`

### Compiling

NOTE: Always use WSL to build the project!

1. `cd` into one of the projects (server or client), like `cd server`
2. Run `cmake . --toolchain=../tc-winwin.cmake` (see footnote for more info).
3. Run `make` to build the project.
4. Run the server with `./server.exe`.

## Mac Instructions

The following environment was tested on a MacBook Pro (2019) environment version 12.6 with the Homebrew package manager.

### Mac Environment Setup

1. Ensure you have [XCode Tools version 12.3](https://developer.apple.com/download/all/) installed. You'll need an Apple Developer account. This is to get access to the OpenGL framework and framework headers.
2. Install _glew_ with `brew install glew`
3. Install _MinGW_ with `brew install mingw-w64`
4. Install _CMake_ with `brew install cmake`

### Compiling for Mac

If you've compiled for Windows, remove the CMake cache first by running `rm -rf CMakeCache.txt CMakeFiles`

1. `cd` into one of the projects (server or client), like `cd server`
2. Run `cmake . --toolchain=../tc-osx.cmake` (see footnote for more info).
3. Run `make` to build the project.
4. Run the server with `./server.exe`.

### Compiling for Windows

As a developer on Mac, it may be useful to know if your code will even compile on Windows. To check, follow these instructions.

If you've compiled for Mac, remove the CMake cache first by running `rm -rf CMakeCache.txt CMakeFiles`

1. `cd` into one of the projects (server or client), like `cd server`
2. Run `cmake . --toolchain=../tc-macwin.cmake` (see footnote for more info).
3. Run `make` to build the project.
4. If the program compiles, there is no further action needed, and there's a good chance the program will compile on a native Windows machine. You will be unable to run the generated `server.exe` executable, but in theory, you would be able to copy the .exe file to a Windows machine and it will run.

\* _What's up with --toolchain?_

The --toolchain flag tells CMake to use the specified toolchain with our custom toolchain file, for example, tc-osx.cmake, where it will use clang and clang++ to compile the files, with the libraries and frameworks specified in the toolchain file. Each environment has its own toolchain: for compiling on Windows: tc-winwin.cmake; for compiling on Mac with a Windows target: tc-macwin.cmake; for compiling on Mac with a Mac target: tc-osx.cmake
