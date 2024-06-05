#include "Particle.hpp"
Particle::Particle() {

}

Particle::Particle(glm::vec3 position, glm::vec3 endPosition, glm::vec3 velocity, glm::vec4 startColor, glm::vec4 endColor, float lifeTime, float size, glm::vec3 direction, ParticleType type, bool isBillboard) 
    : position(position), endPosition(endPosition), velocity(velocity), startColor(startColor), endColor(endColor), lifeTime(lifeTime), size(size), direction(direction), isBillboard(isBillboard) {
    quad = new Quad(position, size);
    quad->setShader(Shader::GetShader(ShaderType::PARTICLE));
    this->active = false;
    this->dSize = size;
    this->type = type;
}

Particle::Particle(glm::vec3 position, glm::vec3 endPosition, glm::vec3 velocity, const char* fileName, const std::string& directory, float lifeTime, float size, glm::vec3 direction, ParticleType type, bool isBillboard) 
    : position(position), endPosition(endPosition), velocity(velocity), lifeTime(lifeTime), size(size), direction(direction), isBillboard(isBillboard) {
    quad = new Quad(position, size);
    quad->setTexture(fileName, directory);
    quad->setShader(Shader::GetShader(ShaderType::PARTICLE));
    this->active = false;
    this->dSize = size;
    this->type = type;
}


Particle::~Particle() {
    delete quad;
}

void Particle::update(float deltaTime) {
    if(type == ParticleType::LINE) {
        position += velocity * deltaTime * direction;
        // position += velocity * deltaTime;
        distanceToEnd = glm::distance(position, endPosition);
        // std::cout<<"move: "<< deltaTime << std::endl;
        dSize += 0.03f;
        // float t = timePassed / lifeTime;
        // position = glm::mix(position, endPosition, t);
        // quad->setRotation(rotation, glm::vec3(1.0f, 0.0f, 0.0f));
    } else if (type == ParticleType::SPREAD) {
        position += velocity * deltaTime;
        dSize = size;
        distanceToEnd = 999;
    }

    timePassed += deltaTime;
    // float t = timePassed / lifeTime;
    // position = glm::mix(position, endPosition, t);
    // std::cout<<"position: " << glm::to_string(position) << std::endl;
    quad->setSize(dSize);
    quad->setPosition(position);
    // quad->setRotation(rotation, glm::vec3(1.0f, 0.0f, 0.0f));
    quad->update();

    // set color
    // scale = glm::mix(10.0f, 40.0f, t);
    // quad->setSize(scale);
    // color = glm::lerp(startColor, endColor, (1 - timePassed)/lifeTime);
    // scale = glm::lerp(1.0f, 0.0f, (1 - timePassed)/lifeTime);
    // color.a = color.a * (1 - timePassed)/lifeTime;
    // quad->setSize(scale);
    // quad->setColor(color);

    if(timePassed > lifeTime || distanceToEnd < threshold) {
        active = false;
    }
}

void Particle::draw(const glm::mat4& viewMtx, const glm::mat4& projMtx) {

    if(isBillboard) {
        glm::mat4 newViewMtx = viewMtx;
        newViewMtx[0][0] = 1.0f;
        newViewMtx[0][1] = 0.0f;
        newViewMtx[0][2] = 0.0f;
        newViewMtx[1][0] = 0.0f;
        newViewMtx[1][1] = 1.0f;
        newViewMtx[1][2] = 0.0f;
        newViewMtx[2][0] = 0.0f;
        newViewMtx[2][1] = 0.0f;
        newViewMtx[2][2] = 1.0f;
        quad->draw(newViewMtx, projMtx);
    } else {
        quad->draw(viewMtx, projMtx);
    }
    // glm::mat4 newViewMtx = viewMtx;
    // newViewMtx[0][0] = 1.0f;
    // newViewMtx[0][1] = 0.0f;
    // newViewMtx[0][2] = 0.0f;
    // newViewMtx[1][0] = 0.0f;
    // newViewMtx[1][1] = 1.0f;
    // newViewMtx[1][2] = 0.0f;
    // newViewMtx[2][0] = 0.0f;
    // newViewMtx[2][1] = 0.0f;
    // newViewMtx[2][2] = 1.0f;
}

void Particle::resetForLine(glm::vec3 position, glm::vec3 velocity) {
    this->position = position;
    this->velocity = velocity + glm::vec3(rand() % 5, rand() % 5, rand() % 5);
    this->timePassed = 0.0f;
    this->color = startColor;
    this->rotation = rand() % 360;
    // std::cout<<"angle: " << rotation << std::endl;
    quad->setRotation(rotation, glm::vec3(rand() % 2, rand() % 2, rand() % 2));
    this->active = true;
    this->dSize = size;
}

void Particle::resetForSpread(glm::vec3 position, glm::vec3 velocity, int range) {
    this->position = position;
    float randX = static_cast<float>(rand() % (2 * range) - range);
    float randY = static_cast<float>(rand() % (2 * range) - range);
    float randZ = static_cast<float>(rand() % (2 * range) - range);
    float randomSize = 0.1f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(size - 0.1f)));
    
    this->velocity = velocity + glm::vec3(randX, randY, randZ);
    // this->velocity = velocity + glm::vec3(rand() % range + 2500, rand() % range + 2500 , rand() % range + 2500);
    
    this->timePassed = 0.0f;
    this->color = startColor;
    this->active = true;
    this->dSize = randomSize;
}