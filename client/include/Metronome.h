#pragma once
#include "core.h"
#include "Shader.h"
#include "Quad.h"
#include "../../_common/include/States.hpp"

class Metronome : public States{
public: 
    Metronome();
    Metronome(int bpm);
    ~Metronome();
    void draw(float aspectRatio);
    void update(float dt);
    void setPosition(glm::vec3 pos);
    void setBpm(int bpm);
private:
    int bpm;
    glm::mat4 modelMtx;
    GLuint shader;
    float beatDuration;
    float currentTime;
    Quad* quad;
    Quad* quad2;

};