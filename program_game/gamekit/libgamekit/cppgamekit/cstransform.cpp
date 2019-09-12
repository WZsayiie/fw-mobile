#include "cstransform.hh"
#include "csgameobject.hh"

cq_member(csTransform) {
    csVector2 position;
};

void csTransform::setPosition(csVector2 position) {
    dat->position = position;
}

csVector2 csTransform::position() {
    return dat->position;
}

void csTransform::setParent(csTransformRef parent) {
    csGameObjectRef object = gameObject();
    if (object == nullptr) {
        return;
    }
    
    if (parent != nullptr) {
        csGameObjectRef parentObject = parent->gameObject();
        object->setParent(parentObject);
    } else {
        object->setParent(nullptr);
    }
}

csTransformRef csTransform::parent() {
    csGameObjectRef object = gameObject();
    if (object == nullptr) {
        return nullptr;
    }
    
    csGameObjectRef parentObject = object->parent();
    if (parentObject != nullptr) {
        return parentObject->transform();
    } else {
        return nullptr;
    }
}