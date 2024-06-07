#include "Endgame.h"

Endgame::Endgame() {
    shader = Shader::GetShader(ShaderType::HUD);

    quad = new Quad(glm::vec3(0, 0, 0), 0.8f);
    quad->setTexture("victory.png", "../assets/HUD/endgame");
    quad->setTexture("defeat.png", "../assets/HUD/endgame");
    quad->disableState(VISIBLE);
    quad->update();
}

Endgame::~Endgame() { delete quad; }

void Endgame::drawVictory(bool didWin) {
    if (didWin) {
        quad->activateTexture(0);
    } else {
        quad->activateTexture(1);
    }
    quad->enableState(VISIBLE);
    quad->update();
}

void Endgame::draw(float aspectRatio) {
    GLuint shader = quad->getShader();
    glUseProgram(shader);

    glUniform1i(glGetUniformLocation(shader, "isHealthBar"), 0);
    if (quad->checkState(VISIBLE))
        quad->draw(aspectRatio);
}