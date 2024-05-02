#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "IComponent.hpp"
#include "../glm/glm.hpp"

class Collider : public IComponent {
    glm::vec3 position = glm::vec3();
    float radius = 0.0f;
    float height = 0.0f;
    float startAngle = 0.0f;
    float endAngle = 0.0f;
    bool isSector = false;
    bool isPoint = false;

public:
    Collider();
    Collider(GameObject* owner);
    // Point, only need position
    Collider(GameObject* owner, glm::vec3 newPosition);
    // Cylinder, no Rotation needed
    Collider(GameObject* owner, glm::vec3 newPosition, glm::vec3 newScale);
    // TODO: CylinderSector
    // Collider(GameObject* owner, glm::vec3 newPosition, glm::vec3 newRotation, glm::vec3 newScale);

    virtual std::string toString() const override;

    const glm::vec3& getPosition() const;
    float getRadius() const;
    float getHeight() const;
    float getStartAngle() const;
    float getEndAngle() const;
    bool getIsSector() const;
    bool getIsPoint() const;

    void setPosition(glm::vec3 newPosition);
    void setRadius(float newRadius);
    void setHeight(float newHeight);
    void setStartAngle(float newStartAngle);
    void setEndAngle(float newEndAngle);
    void setIsSector(bool sector);
    void setIsPoint(bool point);

    // void updateCylinder(glm::vec3 newPosition, float nradius, float nheight);

    // void updateSector(glm::vec3 newPosition, float nradius, float nheight, float nstartAngle, float nendAngle);

    // void updatePoint(glm::vec3 newPosition);
};

#endif // COLLIDER_HPP
