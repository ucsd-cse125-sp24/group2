#ifndef ENTITY_H
#define ENTITY_H

#include "../../_common/include/NetworkObject.hpp"

class Entity : public NetworkObject {
    

public:
    Entity();

    std::string ToString() override;
};
#endif // ENTITY_H