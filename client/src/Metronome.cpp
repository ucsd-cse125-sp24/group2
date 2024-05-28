#include "Metronome.h"


Metronome::Metronome() {
    shader = Shader::GetShader(ShaderType::HUD);
}

Metronome::Metronome(int bpm) : bpm(bpm) {
    beatDuration = 60.0f / float(bpm);
    quad = new Quad(glm::vec3(0.0f, 0.0f, 0.0f), 0.05f);
    quad->setTexture("square-red.png", "../assets/HUD/metronome");
    quad2 = new Quad(glm::vec3(0.0f, 0.0f, 0.0f), 0.1f);
    quad2->setTexture("square-red.png", "../assets/HUD/metronome");
 
}

Metronome::~Metronome() {
    delete quad;
    delete quad2;
}

void Metronome::update(float dt) {
    float ratio = 0.05f/0.1f;
    currentTime += dt;
    quad->update();
    quad2->update();
    if (currentTime > beatDuration) {
        currentTime = 0.0f;
        quad2->setSize(0.1f);
    } else {
        float t = currentTime / beatDuration;
        t = glm::clamp(t, 0.0f, 1.0f);
        glm::vec3 size = glm::mix(glm::vec3(0.1f), glm::vec3(0.05f), t);
        quad2->setSize(size.x);
        // glm::vec3 translation = rectangle1->getPosition();
        // quad2->modelMtx = glm::translate(glm::mat4(1.0f), translation) *
        //                        glm::scale(glm::mat4(1.0f), size) *
        //                        glm::translate(glm::mat4(1.0f), -translation);
        
    }
}

void Metronome::draw(float aspectRatio) {
    // rectangle1->draw(viewProjMtx, shader);
    // rectangle2->draw(viewProjMtx, shader);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLuint shader = quad->getShader();
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "isHealthBar"), 0);
    quad->draw(aspectRatio);
    quad2->draw(aspectRatio);
}

void Metronome::setBpm(int bpm) {
    this->bpm = bpm;
    beatDuration = 60.0f / float(bpm);
}