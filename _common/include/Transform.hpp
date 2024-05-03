#ifndef TRANSFORM_H
#define TRANSFORM_H


#include "IComponent.hpp"
#include "glm/glm.hpp"


class Transform : public IComponent {
    
private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

public:
    Transform();
    Transform(GameObject* owner);
    Transform(GameObject* owner, glm::vec3 newPosition, glm::vec3 newRotation, glm::vec3 newScale);

    virtual std::string ToString();

    const glm::vec3& GetPosition() const { return position; }
    const glm::vec3& GetRotation() const { return rotation; }
    const glm::vec3& GetScale() const { return scale; }

    void SetPosition(glm::vec3 newPosition) { position = newPosition; }
    void SetRotation(glm::vec3 newRotation) { rotation = newRotation; }
    void SetScale(glm::vec3 newScale) { scale = newScale; }
};

#endif // TRANSFORM_H