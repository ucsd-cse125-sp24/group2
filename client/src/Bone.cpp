#include "Bone.h"

Bone::Bone() {

}

Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel) : name(name), id(ID) {
    localMtx = glm::mat4(1.0f);
    numPos = channel->mNumPositionKeys;
    for(int i = 0; i < numPos; i++) {
        aiVector3D pos = channel->mPositionKeys[i].mValue;
        float timeStamp = channel->mPositionKeys[i].mTime;
        KeyPosition key;
        key.position = glm::vec3(pos.x, pos.y, pos.z);
        key.timeStamp = timeStamp;
        positions.push_back(key);
    }

    numRot = channel->mNumRotationKeys;
    for(int i = 0; i < numRot; i++) {
        aiQuaternion rot = channel->mRotationKeys[i].mValue;
        float timeStamp = channel->mRotationKeys[i].mTime;
        KeyRotation data;
        data.orientation = Helper::GetGLMQuat(rot);
        data.timeStamp = timeStamp;
        rotations.push_back(data);
    }

    numScale = channel->mNumScalingKeys;
    for(int i = 0; i < numScale; i++) {
        aiVector3D scale = channel->mScalingKeys[i].mValue;
        float timeStamp = channel->mScalingKeys[i].mTime;
        KeyScale data;
        data.scale = glm::vec3(scale.x, scale.y, scale.z);
        data.timeStamp = timeStamp;
        scales.push_back(data);
    }
}

void Bone::update(float animationTime) {
    glm::mat4 translation = interpolatePosition(animationTime);
    glm::mat4 rotation = interpolateRotation(animationTime);
    glm::mat4 scaling = interpolateScaling(animationTime);
    localMtx = translation * rotation * scaling;
    // std::cout<<"localMtx: "<< glm::to_string(localMtx)<<std::endl;
}

int Bone::getPosIndex(float animationTime) {
    for(int i = 0; i < numPos - 1; i++) {
        if(animationTime < positions[i + 1].timeStamp) {
            return i;
        }
    }
    assert(0);
}

int Bone::getRotIndex(float animationTime) {
    for(int i = 0; i < numRot - 1; i++) {
        if(animationTime < rotations[i + 1].timeStamp) {
            return i;
        }
    }
    assert(0);
}

int Bone::getSclIndex(float animationTime) {
    for(int i = 0; i < numScale - 1; i++) {
        if(animationTime < scales[i + 1].timeStamp) {
            return i;
        }
    }
    assert(0);
}

const glm::mat4& Bone::getLocalTransform() const {
    return localMtx;
}

std::string Bone::getName() const {
    return name;
}

int Bone::getID() const {
    return id;
}

float Bone::getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) {
    float scaleFactor = 0.0f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

glm::mat4 Bone::interpolatePosition(float animationTime) {
    if(numPos == 1) {
        return glm::translate(glm::mat4(1.0f), positions[0].position);
    }
    int p0Index = getPosIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = getScaleFactor(positions[p0Index].timeStamp, positions[p1Index].timeStamp, animationTime);
	glm::vec3 finalPosition = glm::mix(positions[p0Index].position, positions[p1Index].position, scaleFactor);
	return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 Bone::interpolateRotation(float animationTime) {
    if (numRot == 1) {
        auto rotation = glm::normalize(rotations[0].orientation);
        return glm::toMat4(rotation);
	}

    int p0Index = getRotIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = getScaleFactor(rotations[p0Index].timeStamp, rotations[p1Index].timeStamp, animationTime);
    glm::quat finalRotation = glm::slerp(rotations[p0Index].orientation, rotations[p1Index].orientation, scaleFactor);
    finalRotation = glm::normalize(finalRotation);
    return glm::toMat4(finalRotation);
}

glm::mat4 Bone::interpolateScaling(float animationTime) {
    if (numScale == 1)
        return glm::scale(glm::mat4(1.0f), scales[0].scale);

    int p0Index = getSclIndex(animationTime);
    int p1Index = p0Index + 1;
    float scaleFactor = getScaleFactor(scales[p0Index].timeStamp, scales[p1Index].timeStamp, animationTime);
    glm::vec3 finalScale = glm::mix(scales[p0Index].scale, scales[p1Index].scale, scaleFactor);
    return glm::scale(glm::mat4(1.0f), finalScale);
}