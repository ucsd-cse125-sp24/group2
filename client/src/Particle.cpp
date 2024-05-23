#include "Particle.h"

Particle::Particle() {
}

Particle::Particle(glm::vec3 position, glm::vec3 velocity, float lifeTime, float scale, const char* fileName, const std::string& directory) {
    quad = new Quad(position, scale);
    quad->setRotation(-180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    quad->setShader(Shader::GetShader(ShaderType::PARTICLE));
    quad->setTexture(fileName, directory);
    this->position = position;
    this->velocity = velocity;
    this->lifeTime = lifeTime;
    this->scale = scale;
    this->isActive = false;
    this->angle = 0.0f;
    this->angularVelocity = 10.0f * glm::pi<float>() / lifeTime;
}

Particle::Particle(glm::vec3 position, glm::vec3 velocity, float lifeTime, float scale, glm::vec4 color, glm::vec4 endColor) {
    quad = new Quad(position, scale);
    quad->setRotation(-180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    quad->setShader(Shader::GetShader(ShaderType::PARTICLE));
    this->position = position;
    this->velocity = velocity;
    this->lifeTime = lifeTime;
    this->scale = scale;
    this->isActive = false;
    this->startColor = color;
    this->endColor = endColor;
    this->angle = 0.0f;
    this->angularVelocity = 10.0f * glm::pi<float>() / lifeTime;
}

void Particle::update(float dt) {
    angle += angularVelocity * dt;
    float radius = 50.0f;
    // position += velocity * dt;
    position.x =  radius * sin(angle);
    position.z =  radius * cos(angle);
    // set transform
    quad->setPosition(position);
    quad->update();
    timePassed += dt;
    // set color
    color = glm::lerp(startColor, endColor, (1 - timePassed)/lifeTime);
    color.a = color.a * (1 - timePassed)/lifeTime;
    quad->setColor(color);

    if(timePassed > lifeTime || angle >= glm::pi<float>()) {
        isActive = false;
    }
}

void Particle::draw(const glm::mat4& viewProjMtx) {
    quad->draw(viewProjMtx);
}

void Particle::reset(glm::vec3 position, glm::vec3 velocity) {
    this->position = position;
    //this->velocity = volocity;
    this->velocity = velocity + glm::vec3(glm::vec3(rand()%10 - 5, rand() % 10-5, rand()% 10-5));
    this->isActive = true;
    this->timePassed = 0.0f;
    this->color = startColor;
    angle = 0.0f;
}
