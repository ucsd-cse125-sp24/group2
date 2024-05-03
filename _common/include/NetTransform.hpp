#ifndef NET_TRANFORM_H
#define NET_TRANFORM_H


#include "INetworkComponent.hpp"
#include "glm/glm.hpp"


class NetTransform : public INetworkComponent {

private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

public:
    NetTransform();
    NetTransform(NetworkObject* owner);
    NetTransform(NetworkObject* owner, glm::vec3 newPosition, glm::vec3 newRotation, glm::vec3 newScale);

    const glm::vec3& GetPosition() const { return position; }
    const glm::vec3& GetRotation() const { return rotation; }
    const glm::vec3& GetScale() const { return scale; }

    void SetPosition(glm::vec3 newPosition) { position = newPosition; }
    void SetRotation(glm::vec3 newRotation) { rotation = newRotation; }
    void SetScale(glm::vec3 newScale) { scale = newScale; }

    virtual std::string ToString() override;
};


#endif // NET_TRANFORM_H