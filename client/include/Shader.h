#pragma once

#include <stdio.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "core.h"

enum ShaderType { STANDARD, ANIMATED, HUD, SKYBOX, TEST };

class Shader {
private:
    static std::map<ShaderType, GLuint> shaders;

public:
    static GLuint LoadShader(ShaderType type, const char* vertex_file_path,
                             const char* fragment_file_path);
    static GLuint GetShader(ShaderType);
    static void CleanUp();
};