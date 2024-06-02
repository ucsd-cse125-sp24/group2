#include "Status.hpp"

void Status::Update(float deltaTime) {}

std::string Status::ToString() {
    std::string output = "Status:";
    for (auto kvPair : typeToStatusEffect) {
        output += "\n  Type: " + std::to_string(kvPair.first);
        output += "\n    Stacks: " + std::to_string(kvPair.second->stacks);
        output += "\n    currTimer: " + std::to_string(kvPair.second->currTimer);
    }
    return output += "\n";
}