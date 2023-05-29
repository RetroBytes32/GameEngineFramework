#include "transform.h"

Transform::Transform() : 
    
    position(glm::vec3(0, 0, 0)),
    rotation(glm::vec4(0, 0, 0, 1)),
    scale(glm::vec3(1, 1, 1)),
    
    matrix(glm::mat4(1)),
    
    mParent(nullptr)
{
}

void Transform::SetPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
    UpdateMatrix();
    return;
}

void Transform::SetPosition(glm::vec3 newPosition) {
    position = glm::vec3(newPosition.x, newPosition.y, newPosition.z);
    UpdateMatrix();
    return;
}

void Transform::SetRotation(float x, float y, float z, float w) {
    rotation = glm::vec4(x, y, z, w);
    UpdateMatrix();
    return;
}

void Transform::SetRotation(glm::vec4 newRotation) {
    rotation = glm::vec4(newRotation.x, newRotation.y, newRotation.z, newRotation.w);
    UpdateMatrix();
    return;
}

void Transform::SetScale(float x, float y, float z) {
    scale = glm::vec3(x, y, z);
    UpdateMatrix();
    return;
}

void Transform::SetScale(glm::vec3 newScale) {
    scale = glm::vec3(newScale.x, newScale.y, newScale.z);
    UpdateMatrix();
    return;
}

glm::vec3 Transform::GetPosition(void) {
    return position;
}

glm::vec4 Transform::GetRotation(void) {
    return rotation;
}

glm::vec3 Transform::GetScale(void){
    return scale;
}

void Transform::ChildAdd(Transform* transform) {
    mChildList.push_back(transform);
    return;
}

bool Transform::ChildRemove(Transform* transform) {
    for (std::vector<Transform*>::iterator it = mChildList.begin(); it != mChildList.end(); ++it) {
        Transform* transformPtr = *it;
        if (transform == transformPtr) {
            mChildList.erase(it);
            return true;
        }
    }
    return false;    
}

void Transform::SetParentTransform(Transform* parentTransform) {
    mParent = parentTransform;
    return;
}

Transform* Transform::GetParentTransform(void) {
    return mParent;
}

Transform Transform::Identity(void) {
    Transform identity = Transform();
    return identity;
}

void Transform::UpdateMatrix(void) {
    
    glm::mat4 modelPosition = glm::translate(glm::mat4(1.0f), glm::vec3( position.x, position.y, position.z ));
    
    glm::mat4 
    modelRotation = glm::rotate (glm::mat4(1.0f), glm::radians( 0.0f ), glm::vec3(0, 1, 0));
    modelRotation = glm::rotate(modelRotation, rotation.x, glm::vec3( 0.0f, 1.0f, 0.0f ) );
    modelRotation = glm::rotate(modelRotation, rotation.y, glm::vec3( 1.0f, 0.0f, 0.0f ) );
    modelRotation = glm::rotate(modelRotation, rotation.z, glm::vec3( 0.0f, 0.0f, 1.0f ) );
    
    glm::mat4 modelScale = glm::scale(glm::mat4(1.0f), glm::vec3( scale.x, scale.y, scale.z ));
    
    matrix = modelPosition * modelRotation * modelScale;
    return;
}
