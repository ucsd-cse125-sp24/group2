#include "Metronome.h"


Metronome::Metronome() {
    shader = Shader::GetShader(ShaderType::HUD);
}

Metronome::Metronome(int bpm) : bpm(bpm) {
    beatDuration = 60000.0f / float(bpm);
    min = 0.05f;
    max = 0.15f;
    quad = new Quad(glm::vec3(0.0f, 0.0f, 0.0f), min);
    quad->setTexture("square-red.png", "../assets/HUD/metronome");
    quad2 = new Quad(glm::vec3(0.0f, 0.0f, 0.0f), max);
    quad2->setTexture("square-red.png", "../assets/HUD/metronome");
    // set initial size
    quad->update();
    quad2->update();
 
}

Metronome::~Metronome() {
    delete quad;
    delete quad2;
}

void Metronome::update(float dt) {
    // std::cout<<"dt: "<<dt<<std::endl;
    if(AudioManager::instance().isStarted()) {
        float currentTime = AudioManager::instance().getPosition();
        float timeInCurrentBeat = fmod(currentTime, beatDuration);
        float t = timeInCurrentBeat / beatDuration;
        t = glm::clamp(t, 0.0f, 1.0f);
        glm::vec3 size = glm::mix(glm::vec3(max), glm::vec3(min), t);
        quad2->setSize(size.x);
        quad->update();
        quad2->update();
        // float ratio = min/max;
        // float adjustSpeed = 0.5f;
        // currentTime += dt * adjustSpeed;
        // quad->update();
        // quad2->update();
        // if (currentTime > beatDuration) {
        //     currentTime = 0.0f;
        //     quad2->setSize(max);
        // } else {
        //     float t = currentTime / beatDuration;

        //     t = glm::clamp(t, 0.0f, 1.0f);
        //     glm::vec3 size = glm::mix(glm::vec3(max), glm::vec3(min), t);
        //     quad2->setSize(size.x);
        // }
    }
}

void Metronome::draw(float aspectRatio) {
    GLuint shader = quad->getShader();
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "isHealthBar"), 0);
    quad->draw(aspectRatio);
    quad2->draw(aspectRatio);
}

void Metronome::setBpm(int bpm) {
    this->bpm = bpm;
    beatDuration = 60.0f / float(bpm);
    this->enableState(VISIBLE);
}