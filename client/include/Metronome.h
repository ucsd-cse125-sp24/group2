#pragma once
#include "core.h"
#include "Shader.h"
#include "Quad.h"

class Metronome {
public: 
    Metronome();
    Metronome(float bpm);
    ~Metronome();
    void draw(float aspectRatio);
    void update(float dt);
    void setPosition(glm::vec3 pos);
private:
    float bpm;
    glm::mat4 modelMtx;
    GLuint shader;
    float beatDuration;
    float currentTime;
    Quad* quad;
    Quad* quad2;

};