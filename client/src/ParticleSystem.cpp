#include "ParticleSystem.hpp"


ParticleSystem::ParticleSystem() {

}

ParticleSystem::ParticleSystem(glm::vec3 velocity, float lifeTime, float size, int numParticles, float rate, const char* fileName, const std::string& directory, ParticleType type, bool isBillboard) 
    : velocity(velocity), numParticles(numParticles), rate(rate), type(type){
    for(int i = 0; i < numParticles; i++) {
        particles.push_back(new Particle(glm::vec3(0.0f), glm::vec3(0.0f), velocity, fileName, directory, lifeTime, size, direction, type, isBillboard));
    }
    index = numParticles - 1;
}

ParticleSystem::ParticleSystem(glm::vec3 velocity, float lifeTime, float size, int numParticles, float rate, glm::vec4 startColor, glm::vec4 endColor, ParticleType type, bool isBillboard) 
    : velocity(velocity), numParticles(numParticles), rate(rate), type(type) {
    for(int i = 0; i < numParticles; i++) {
        particles.push_back(new Particle(glm::vec3(0.0f), glm::vec3(0.0f), velocity, startColor, endColor, lifeTime, size, direction, type, isBillboard));
    }
    index = numParticles - 1;
}


ParticleSystem::ParticleSystem(glm::vec3 startPosition, glm::vec3 endPosition, glm::vec3 velocity, glm::vec4 startColor, glm::vec4 endColor, float lifeTime, float size, int numParticles, float rate, ParticleType type, bool isBillboard) 
    : startPosition(startPosition), rate(rate), numParticles(numParticles), velocity(velocity), type(type) { 
    direction = glm::normalize(endPosition - startPosition);
    for(int i = 0; i < numParticles; i++) {
        particles.push_back(new Particle(startPosition, endPosition, velocity, "note_5.png", "../assets/effects", lifeTime, size, direction, type, isBillboard));
    }
    index = numParticles - 1;
}

ParticleSystem::~ParticleSystem() {
    for(int i = 0; i < particles.size(); i++) {
        delete particles[i];
    }
}
    
void ParticleSystem::update(float deltaTime) {
    if(!isFinished) {
        for(int i = 0; i < particles.size(); i++) {
            if(!particles[i]->isActive()) {
                continue;
            }
            particles[i]->update(deltaTime);
        }
    }
}

void ParticleSystem::draw(const glm::mat4& viewMtx, const glm::mat4& projMtx) {
    if(!isFinished) {
        for(int i = 0; i < particles.size(); i++) {
                if(!particles[i]->isActive()) {
                    continue;
                }
                particles[i]->draw(viewMtx, projMtx);
            }
    }
    
}
// int count = 0;
void ParticleSystem::spawnParticles(glm::vec3 position, ParticleType type) {
    // std::cout<<"SPAWN PARTICLES"<<std::endl;
    Particle* p = particles[index];
    if(p->isActive()) {
        return;
    }
    // count++;
    // std::cout<<"numb particles spawn: " << count << std::endl;
    if(type == ParticleType::LINE)
        p->resetForLine(position, velocity);
    else if(type == ParticleType::SPREAD)
        p->resetForSpread(position, velocity, 7000);
    index = (index - 1 + numParticles) % numParticles;
}

void ParticleSystem::start(float deltaTime, ParticleType type, float duration) {
    elapsedTime += deltaTime;
    // std::cout<<"elapsed time: " << elapsedTime << " duration: " << duration << std::endl;
    if(elapsedTime >= duration) {
        isFinished = true;
        return;
    }
    timeSinceLastSpawn += deltaTime;
    if (timeSinceLastSpawn >= rate) {
        // std::cout<<"CALLED"<<std::endl;
        spawnParticles(startPosition, type);
        timeSinceLastSpawn = 0.0f;
    }
}

void ParticleSystem::setData(glm::vec3 startPosition, glm::vec3 targetPosition, glm::vec3 velocity) {
    this->startPosition = startPosition;
    this->targetPosition = targetPosition;
    this->velocity = velocity;
    // std::cout<<"velocity: " << glm::to_string(velocity) << std::endl;
    direction = glm::normalize(targetPosition - startPosition);
    for(int i = 0; i < particles.size(); i++) {
        particles[i]->setEndPosition(targetPosition);
        particles[i]->setVelocity(velocity);
        particles[i]->setDirection(direction);
    }
}

void ParticleSystem::reset() {
    for(int i = 0; i < particles.size(); i++) {
        particles[i]->setActive(false);
    }
    index = numParticles - 1;
    elapsedTime = 0.0f;
    timeSinceLastSpawn = 0.0f;
    isFinished = false;
}
