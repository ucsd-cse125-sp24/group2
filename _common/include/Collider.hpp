#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "IComponent.hpp"
#include "glm/glm.hpp"
#include "Transform.hpp"

class Collider : public IComponent {
    glm::vec3 position = glm::vec3();
    glm::vec3 rotation = glm::vec3();
    float radius = 0.0f;
    float height = 0.0f;
    float startAngle = 0.0f;
    float endAngle = 0.0f;
    bool isSector = false;
    bool isPoint = false;

public:
    // default, Cylinder with owner at 0,0,0. Can be used for obstacles
    Collider(GameObject* owner);
    // default constructor for cylinder, use functions to change it to
    // sectors/point
    Collider(GameObject* owner, Transform* transform);

    Collider(GameObject* owner, Collider* copyfrom);

    virtual std::string ToString();

    const glm::vec3& GetPosition() const;
    const glm::vec3& GetRotation() const;
    float GetRadius() const;
    float GetHeight() const;
    float GetStartAngle() const;
    float GetEndAngle() const;
    bool GetIsSector() const;
    bool GetIsPoint() const;

    void SetPosition(glm::vec3 newPosition);
    void SetRotation(glm::vec3 newRotation);
    void SetRadius(float newRadius);
    void SetHeight(float newHeight);
    void SetStartAngle(float newStartAngle);
    void SetEndAngle(float newEndAngle);
    void SetIsSector(bool sector);
    void SetIsPoint(bool point);

    // input: angle: the whole angle range of the sector
    void makeSector(float angle);

    void makePoint();
};

#endif // COLLIDER_HPP
