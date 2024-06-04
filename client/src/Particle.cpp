#include "Particle.hpp"

Particle::Particle() {

}

Particle::Particle(glm::vec3 position, glm::vec3 velocity, glm::vec4 startColor, glm::vec4 endColor, float lifeTime, float size) 
    : position(position), velocity(velocity), startColor(startColor), endColor(endColor), lifeTime(lifeTime), size(size) {
    quad = new Quad(position, size);
    quad->setShader(Shader::GetShader(ShaderType::PARTICLE));
    this->active = false;
}

Particle::~Particle() {
    delete quad;
}

void Particle::update(float deltaTime) {
    position += velocity * deltaTime;
    quad->setPosition(position);
    quad->update();

    timePassed += deltaTime;
    // set color
    color = glm::lerp(startColor, endColor, (1 - timePassed)/lifeTime);
    color.a = color.a * (1 - timePassed)/lifeTime;
    quad->setColor(color);

    if(timePassed > lifeTime) {
        active = false;
    }
}

void Particle::draw(const glm::mat4& viewProjMtx) {
    quad->draw(viewProjMtx);
}

void Particle::reset(glm::vec3 position) {
    this->position = position;
    this->velocity = velocity + glm::vec3(rand() % 2, rand() % 2, rand() % 2);
    this->timePassed = 0.0f;
    this->color = startColor;
    this->active = true;
}