#include "../include/Entity.h"
#include "../../_common/include/NetTransform.hpp"
#include "../include/Mover.h"
#include "../include/Model.h"
#include "../include/AnimationClip.h"
#include "../include/AnimationPlayer.h"


Entity::Entity() : NetworkObject() {
    // Still need to refactor to make constructors take owner arguments
    
    // Mover* mover = new Mover();
    // AddComponent(mover);
    // Model* model = new Model("../assets/male_basic_walk_30_frames_loop/scene.gltf");
    // AddComponent(model);
    // AnimationClip* clip = new AnimationClip();
    // AddComponent(clip);
    // AnimationPlayer* animPlayer = new AnimationPlayer();
    // AddComponent(animPlayer);
    // EntityManager?
}

std::string Entity::ToString() {
    // probably print the name and id of this entity and its components
    return "Entity";
}