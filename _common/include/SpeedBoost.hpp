#pragma once


#include "IStatusEffect.hpp"
#include <iostream>


class SpeedBoost : public IStatusEffect {
public:
    SpeedBoost(Status* status) : IStatusEffect(status, 15.0f, true) {}

    StatusEffectTypeID TypeID() override { return SPEED_BOOST; }
    void OnUpdate(float deltaTime) override { 
        // std::cout << "baseSpeed w/ boost: " << status->baseSpeed << std::endl; 
    }
    void OnAdd() override { 
        // std::cout << "OnAdd(): Updating speed from: " << status->baseSpeed << std::endl;
        status->baseSpeed += 5.0f; 
        // std::cout << "To: " << status->baseSpeed << std::endl;
    }
    void OnRemove() override { 
        // std::cout << "OnRemove(): Updating speed from: " << status->baseSpeed << std::endl;
        status->baseSpeed -= 5.0f; 
        // std::cout << "To: " << status->baseSpeed << std::endl;
    }
};