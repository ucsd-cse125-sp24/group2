# Group2Good Project

## Environment Setup Instructions

1. Install [WSL](https://learn.microsoft.com/en-us/windows/wsl/install)
2. Launch WSL
3. Install mingw-w64 by running `sudo apt-get install mingw-w64`
4. Add the mingw g++ compiler environment variable by running `export CC=x86_64-w64-mingw32-g++`, and/or add it to your `.bashrc`.
5. You can now run `make` to build the projects.
