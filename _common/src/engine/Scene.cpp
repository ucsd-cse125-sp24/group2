#include "engine/Scene.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Health.hpp"
#include <algorithm>

void Scene::Update(float deltaTime) {
    int alive = 0;
    bool enemyAlive = true;
    for (auto const& entity : entities) {

        // If it's a player or enemy, check if alive
        // TOOD: destroy dead player object?
        Player* p = dynamic_cast<Player*>(entity);
        if(p != nullptr){
            alive = p->alive ? alive + 1 : alive;
        }
        else{
            Enemy* e = dynamic_cast<Enemy*>(entity);
            if(e != nullptr){
                enemyAlive = e->GetComponent<Health>()->GetHealth() > 0;

                // if enemy is dead, break and "end"
                if(!enemyAlive)
                    break;
            }
        }

        // otherwise, if game and current entity are active, update
        if(gameActive){
            entity->update(deltaTime);
            for (int i = 0; i < entity->components.size(); i++) {
                entity->components[i]->Update(deltaTime);
            }
            for (int i = 0; i < entity->networkComponents.size(); i++) {
                entity->networkComponents[i]->Update(deltaTime);
            }
        }
    }

    if(gameActive){
        // Check losing condition
        if(alive == 0){
            printf("YOU LOSE :[\n");
            gameActive = false;
            // TODO: end the game
        }

        // Check win condition
        if(!enemyAlive){
            printf("YOU WIN :D\n");
            gameActive = false;
            // TODO: end the game
        }
    }

}

void Scene::Instantiate(Entity* e) {
    entities.push_back(e);
    ObjectEventArgs* args = new ObjectEventArgs(e);
    object_added.invoke(args);
}

// TODO: Update client states when object is removed
void Scene::Destroy(Entity* e) {
    entities.erase(std::remove(entities.begin(), entities.end(), e),
                   entities.end());
    ObjectEventArgs* args = new ObjectEventArgs(e);
    object_removed.invoke(args);
}