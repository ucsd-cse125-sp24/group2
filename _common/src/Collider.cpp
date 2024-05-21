#include "Collider.hpp"
#include "glm/gtx/string_cast.hpp"

Collider::Collider(GameObject* owner) : IComponent(owner) {}

Collider::Collider(GameObject* owner, NetTransform* transform)
    : IComponent(owner), position(transform->GetPosition()), radius(transform->GetScale().x),
      height(transform->GetScale().z), rotation(transform->GetRotation()) {}

// input: angle: the whole angle range of the sector
void Collider::makeSector(float angle) {
    if (isSector)
        return;
    glm::vec3 projectedVec(rotation.x, rotation.y, 0.0f);
    glm::vec3 normalizedVec = glm::normalize(projectedVec);
    glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
    float dot = glm::dot(normalizedVec, xAxis);
    float rotationAngle = std::acos(dot);
    startAngle = rotationAngle - angle / 2;
    endAngle = rotationAngle + angle / 2;
    isPoint = false;
    isSector = true;
}

void Collider::makePoint() { 
    isPoint = true;
    isSector = false;
    radius = 0.0f;
    height = 0.0f;
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
void Collider::SetStartAngle(float newStartAngle) {
    startAngle = newStartAngle;
}
void Collider::SetEndAngle(float newEndAngle) { endAngle = newEndAngle; }
void Collider::SetIsSector(bool sector) { isSector = sector; }
void Collider::SetIsPoint(bool point) { isPoint = point; }