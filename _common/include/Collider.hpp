#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "IComponent.hpp"
#include "glm/glm.hpp"

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
    // default constructor for cylinder, use functions to change it to sectors/point
    Collider(GameObject* owner, glm::vec3 newPosition, glm::vec3 newRotation, glm::vec3 newScale);

    virtual std::string toString() const override;

    const glm::vec3& getPosition() const;
    const glm::vec3& getRotation() const;
    float getRadius() const;
    float getHeight() const;
    float getStartAngle() const;
    float getEndAngle() const;
    bool getIsSector() const;
    bool getIsPoint() const;

    void setPosition(glm::vec3 newPosition);
    void setRotation(glm::vec3 newRotation);
    void setRadius(float newRadius);
    void setHeight(float newHeight);
    void setStartAngle(float newStartAngle);
    void setEndAngle(float newEndAngle);
    void setIsSector(bool sector);
    void setIsPoint(bool point);

    void makeSector(float angle);

    void makePoint();

    // void updateCylinder(glm::vec3 newPosition, float nradius, float nheight);

    // void updateSector(glm::vec3 newPosition, float nradius, float nheight, float nstartAngle, float nendAngle);

    // void updatePoint(glm::vec3 newPosition);
};

#endif // COLLIDER_HPP
