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

std::string Collider::ToString() {
    return "Position: " + glm::to_string(position) +
           ", Radius: " + std::to_string(radius) +
           ", Height: " + std::to_string(height) +
           ", Start Angle: " + std::to_string(startAngle) +
           ", End Angle: " + std::to_string(endAngle) +
           ", Sector: " + (isSector ? "Yes" : "No") +
           ", Point: " + (isPoint ? "Yes" : "No");
}

const glm::vec3& Collider::GetPosition() const { return position; }
const glm::vec3& Collider::GetRotation() const { return rotation; }
float Collider::GetRadius() const { return radius; }
float Collider::GetHeight() const { return height; }
float Collider::GetStartAngle() const { return startAngle; }
float Collider::GetEndAngle() const { return endAngle; }
bool Collider::GetIsSector() const { return isSector; }
bool Collider::GetIsPoint() const { return isPoint; }

void Collider::SetPosition(glm::vec3 newPosition) { position = newPosition; }
void Collider::SetRotation(glm::vec3 newRotation) { rotation = newRotation; }
void Collider::SetRadius(float newRadius) { radius = newRadius; }
void Collider::SetHeight(float newHeight) { height = newHeight; }
void Collider::SetStartAngle(float newStartAngle) { startAngle = newStartAngle; }
void Collider::SetEndAngle(float newEndAngle) { endAngle = newEndAngle; }
void Collider::SetIsSector(bool sector) { isSector = sector; }
void Collider::SetIsPoint(bool point) { isPoint = point; }


// void Collider::updateCylinder(glm::vec3 newPosition, float nradius, float nheight) {
//     SetPosition(newPosition); 
//     SetRadius(nradius); 
//     SetHeight(nheight);
// }

// void Collider::updateSector(glm::vec3 newPosition, float nradius, float nheight, float nstartAngle, float nendAngle) {
//     SetPosition(newPosition); 
//     SetRadius(nradius); 
//     SetHeight(nheight);
//     SetStartAngle(nstartAngle); 
//     SetEndAngle(nendAngle);
// }

// void Collider::updatePoint(glm::vec3 newPosition) {
//     SetPosition(newPosition);
// }