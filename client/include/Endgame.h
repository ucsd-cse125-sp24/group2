#pragma once
#include "core.h"
#include "Quad.h"

class Endgame : public States {
public:
    Endgame();
    ~Endgame();
    void draw(float aspectRatio);
    void drawVictory(bool didWin);

private:
    glm::mat4 modelMtx;
    GLuint shader;
    Quad* quad;
};