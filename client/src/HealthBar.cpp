#include "HealthBar.h"
#include "Helper.h"

HealthBar::HealthBar() {
   
}
HealthBar::HealthBar(glm::vec3 pos, float width, float height) {
    quad = new Quad(pos, width, height);
    quad->setTexture("emptyBar.png", "../assets/HUD/healthBar");
    quad->setTexture("fullBar.png", "../assets/HUD/healthBar");

    ratio = currHealth / maxHealth;
}

HealthBar::HealthBar(glm::vec3 pos, float size) {
    quad = new Quad(pos, size);
    ratio = currHealth / maxHealth;
    quad->setTexture("emptyBar.png", "../assets/HUD/healthBar");
    quad->setTexture("fullBar.png", "../assets/HUD/healthBar");
}

HealthBar::~HealthBar() {
    delete quad;
}

void HealthBar::draw(float aspectRatio) {
    // remove background
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if(currHealth < 0) {
        currHealth = 0;
    }
    if(currHealth > maxHealth) {
        currHealth = maxHealth;
    }
    GLuint shader = quad->getShader();
    ratio = currHealth / maxHealth;
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "isHealthBar"), 1);
    glUniform1f(glGetUniformLocation(shader, "ratio"), ratio);
    quad->draw(aspectRatio);
}

void HealthBar::update() {
    quad->update();
}