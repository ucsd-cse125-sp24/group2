#ifndef ENTITY_H
#define ENTITY_H
class Entity {
    int network_id;
   public:
    virtual void update() = 0;
};

#endif  // ENTITY_H