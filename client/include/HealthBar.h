#pragma once

#include "core.h"
#include <stb_image.h>
#include <iostream>
#include "Quad.h"
#include "../../_common/include/States.hpp"

class HealthBar : public States{
public:
    HealthBar();
    HealthBar(glm::vec3 pos, float size);
    HealthBar(glm::vec3 pos, float width, float height);
    ~HealthBar();
    void draw(float aspectRatio);
    void setHealth(float health) { currHealth = health; }
    void setMaxHealth(float health) { maxHealth = health; }
    float getHealth() const { return currHealth; }   
    void update();
    float currHealth = 100.0f;
private:
    Quad* quad;
    // float currHealth = 100.0f;
    float maxHealth = 100.0f;
    float ratio = 0.0f;
};
