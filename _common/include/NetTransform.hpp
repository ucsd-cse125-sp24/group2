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
    NetTransform(NetworkObject* owner) : INetworkComponent(owner) {}
    NetTransform(NetworkObject* owner, glm::vec3 newPosition, glm::vec3 newRotation, glm::vec3 newScale) : 
        INetworkComponent(owner),
        position(newPosition),
        rotation(newRotation),
        scale(newScale)
    {}

    virtual std::string toString() const;

    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getRotation() const { return rotation; }
    const glm::vec3& getScale() const { return scale; }

    void setPosition(glm::vec3 newPosition) { position = newPosition; }
    void setRotation(glm::vec3 newRotation) { rotation = newRotation; }
    void setScale(glm::vec3 newScale) { scale = newScale; }
};


#endif // NET_TRANFORM_H