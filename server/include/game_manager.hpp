#include <vector>
#include "entity.hpp"
class GameManager {
    std::vector<Entity*> entities;

   public:
    void update();
    void register_entity(Entity*);
};