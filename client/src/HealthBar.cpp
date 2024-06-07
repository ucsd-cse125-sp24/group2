#include "HealthBar.h"
#include "Helper.h"

HealthBar::HealthBar() {}
HealthBar::HealthBar(glm::vec3 pos, float width, float height) {
    quad = new Quad(pos, width, height);
    //quad->setTexture("empty.png", "../assets/HUD/healthBar");
    //quad->setTexture("full.png", "../assets/HUD/healthBar");

    ratio = currHealth / maxHealth;
}

HealthBar::HealthBar(glm::vec3 pos, float size) {
    quad = new Quad(pos, size);
    ratio = currHealth / maxHealth;
    // quad->setTexture("empty.png", "../assets/HUD/healthBar");
    // quad->setTexture("full.png", "../assets/HUD/healthBar");
    quad->update();
    
    quad->enableState(VISIBLE);
}

HealthBar::~HealthBar() { delete quad; }

void HealthBar::draw(float aspectRatio) {
    // remove background
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (currHealth < 0) {
        currHealth = 0;
    }
    if (currHealth > maxHealth) {
        currHealth = maxHealth;
    }
    GLuint shader = quad->getShader();
    
    ratio = ((currHealth) / (maxHealth));

    //std::cout<<"ratio" << ratio << std::endl;
    //std::cout<<"heath: " << currHealth <<std::endl;
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "isHealthBar"), 1);
    glUniform1f(glGetUniformLocation(shader, "ratio"), ratio);
    quad->draw(aspectRatio);
}

void HealthBar::update() { quad->update(); }

void HealthBar::enableState(ObjectStates state) {
    quad->enableState(state);
}

void HealthBar::disableState(ObjectStates state) {
    quad->disableState(state);
}

bool HealthBar::checkState(ObjectStates state) {
    return quad->checkState(state);
}

void HealthBar::setTexture(const char* path, const std::string& directory) {
    quad->setTexture(path, directory);
}