#include "Metronome.h"

Metronome::Metronome() {
    shader = Shader::GetShader(ShaderType::HUD);
    quad2->update();
}

Metronome::Metronome(int bpm) : bpm(bpm) {
    beatDuration = 60000 / (float)bpm;
    std::cout << "bpm: " << beatDuration << std::endl;
    min = 0.05f;
    max = 0.15f;
    // quad = new Quad(glm::vec3(0.0f, 0.0f, 0.0f), max);
    // quad->setTexture("square-red.png", "../assets/HUD/metronome");
    quad2 = new Quad(glm::vec3(0.0f, 0.0f, 0.0f), max);
    quad2->setTexture("square-red-thick.png", "../assets/HUD/metronome");

    quad2->enableState(VISIBLE);

    quad2->update();
}

Metronome::~Metronome() {
    // delete quad;
    delete quad2;
}

void Metronome::update() {
    // std::cout<<"dt: "<<dt<<std::endl;
    if (AudioManager::instance().getPosition() > 0) {
        unsigned int currentTime = AudioManager::instance().getPosition();
        // std::cout<<"beatDuration: "<<beatDuration<<std::endl;
        // std::cout<<"curr/duration:" << currentTime/517<<std::endl;
        float timeInCurrentBeat = fmod(currentTime, beatDuration * 2);
        float t = timeInCurrentBeat / (beatDuration);
        // float t = (currentTime / (beatDuration ) - floor(currentTime /
        // (beatDuration )));
        t = glm::clamp(t, 0.0f, 1.0f);
        // std::cout<<"t: "<<t<<std::endl;

        glm::vec3 size = glm::mix(glm::vec3(max), glm::vec3(min), t);

        quad2->setSize(size.x);

        quad2->update();
    }
}

void Metronome::draw(float aspectRatio) {
    GLuint shader = quad2->getShader();
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "isHealthBar"), 0);
    quad2->draw(aspectRatio);
}

void Metronome::setBpm(int bpm) {
    this->bpm = bpm;
    beatDuration = 60000 / (float)bpm;
    this->enableState(VISIBLE);
}