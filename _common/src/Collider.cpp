#include "Collider.hpp"
#include "glm/gtx/string_cast.hpp"

Collider::Collider(GameObject* owner) : IComponent(owner) {}

Collider::Collider(GameObject* owner, glm::vec3 newPosition, glm::vec3 newRotation, glm::vec3 newScale)
: IComponent(owner), position(newPosition), radius(newScale.x), height(newScale.z), rotation(newRotation) {}

void Collider::makeSector(float angle) {
    if (isSector)
        return;
    glm::vec3 projectedVec(rotation.x, rotation.y, 0.0f);
    glm::vec3 normalizedVec = glm::normalize(projectedVec);
    glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
    float dot = glm::dot(normalizedVec, xAxis);
    float rotationAngle = std::acos(dot);
    startAngle = rotationAngle - angle/2;
    endAngle = rotationAngle + angle/2;
    isSector = true;
}

void Collider::makePoint() {
    isPoint = true;
}

std::string Collider::toString() const {
    return "Position: " + glm::to_string(position) +
           ", Radius: " + std::to_string(radius) +
           ", Height: " + std::to_string(height) +
           ", Start Angle: " + std::to_string(startAngle) +
           ", End Angle: " + std::to_string(endAngle) +
           ", Sector: " + (isSector ? "Yes" : "No") +
           ", Point: " + (isPoint ? "Yes" : "No");
}

const glm::vec3& Collider::getPosition() const { return position; }
const glm::vec3& Collider::getRotation() const { return rotation; }
float Collider::getRadius() const { return radius; }
float Collider::getHeight() const { return height; }
float Collider::getStartAngle() const { return startAngle; }
float Collider::getEndAngle() const { return endAngle; }
bool Collider::getIsSector() const { return isSector; }
bool Collider::getIsPoint() const { return isPoint; }

void Collider::setPosition(glm::vec3 newPosition) { position = newPosition; }
void Collider::setRotation(glm::vec3 newRotation) { rotation = newRotation; }
void Collider::setRadius(float newRadius) { radius = newRadius; }
void Collider::setHeight(float newHeight) { height = newHeight; }
void Collider::setStartAngle(float newStartAngle) { startAngle = newStartAngle; }
void Collider::setEndAngle(float newEndAngle) { endAngle = newEndAngle; }
void Collider::setIsSector(bool sector) { isSector = sector; }
void Collider::setIsPoint(bool point) { isPoint = point; }


// void Collider::updateCylinder(glm::vec3 newPosition, float nradius, float nheight) {
//     setPosition(newPosition); 
//     setRadius(nradius); 
//     setHeight(nheight);
// }

// void Collider::updateSector(glm::vec3 newPosition, float nradius, float nheight, float nstartAngle, float nendAngle) {
//     setPosition(newPosition); 
//     setRadius(nradius); 
//     setHeight(nheight);
//     setStartAngle(nstartAngle); 
//     setEndAngle(nendAngle);
// }

// void Collider::updatePoint(glm::vec3 newPosition) {
//     setPosition(newPosition);
// }