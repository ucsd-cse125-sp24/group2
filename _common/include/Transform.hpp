#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "IComponent.hpp"
#include "glm/glm.hpp"

class Transform : public IComponent {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

public:
    Transform();
    Transform(GameObject* owner);
    Transform(GameObject* owner, glm::vec3 newPosition, glm::vec3 newRotation,
              glm::vec3 newScale);

    virtual std::string toString() const;

    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getRotation() const { return rotation; }
    const glm::vec3& getScale() const { return scale; }

    void setPosition(glm::vec3 newPosition) { position = newPosition; }
    void setRotation(glm::vec3 newRotation) { rotation = newRotation; }
    void setScale(glm::vec3 newScale) { scale = newScale; }
};

#endif // TRANSFORM_H