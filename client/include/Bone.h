#pragma once

#include "core.h"
#include "Helper.h"
#include <iostream>
#include <vector>

struct KeyPosition
{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation
{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

class Bone {
public:
    Bone();
    Bone(const std::string& name, int ID, const aiNodeAnim* channel);
    void update(float animationTime);
    int getPosIndex(float animationTime);
    int getRotIndex(float animationTime);
    int getSclIndex(float animationTime);
    const glm::mat4& getLocalTransform() const;
    std::string getName() const;
    int getID() const;
private:
    float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
    glm::mat4 interpolatePosition(float animationTime);
    glm::mat4 interpolateRotation(float animationTime);
    glm::mat4 interpolateScaling(float animationTime);

    std::vector<KeyPosition> positions;
    std::vector<KeyRotation> rotations;
    std::vector<KeyScale> scales;
    int numPos;
    int numRot;
    int numScale;
    glm::mat4 localMtx;
    std::string name;
    int id;
};