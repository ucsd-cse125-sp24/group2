#pragma once
#include "core.h"
#include "Shader.h"
#include "Quad.h"
#include "../../_common/include/States.hpp"
#include "AudioManager.hpp"

class Metronome : public States{
public: 
    Metronome();
    Metronome(int bpm);
    ~Metronome();
    void draw(float aspectRatio);
    void update();
    void setPosition(glm::vec3 pos);
    void setBpm(int bpm);
private:
    int bpm = 0;
    glm::mat4 modelMtx;
    GLuint shader;
    float beatDuration = 0;
    Quad* quad2;
    float min = 0.0f;
    float max = 0.0f;
    float sumTime = 0.0f;
    Quad* text;
};