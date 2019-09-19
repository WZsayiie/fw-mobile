#include "csgkobject.hh"
#include "csgameobject.hh"

cq_member(csGKObject) {
};

void csGKObject::dontDestroyOnLoad(csGKObjectRef object) {
    if (object == nullptr) {
        return;
    }
    
    if (object->isKindOfClass(csComponent::clazz())) {
        
        auto component = cqObject::cast<csComponent>(object);
        auto gameObject = component->gameObject();
        csGameObject::dontDestoryOnLoad(gameObject);
        
    } else if (object->isKindOfClass(csGameObject::clazz())) {
        
        auto gameObject = cqObject::cast<csGameObject>(object);
        csGameObject::dontDestoryOnLoad(gameObject);
    }
}

void csGKObject::destroy(csGKObjectRef object) {
    if (object == nullptr) {
        return;
    }
    
    if (object->isKindOfClass(csComponent::clazz())) {
        
        auto component = cqObject::cast<csComponent>(object);
        auto gameObject = component->gameObject();
        if (gameObject != nullptr) {
            gameObject->removeComponent(object->dynamicClass());
        }
        
    } else if (object->isKindOfClass(csGameObject::clazz())) {
        
        auto gameObject = cqObject::cast<csGameObject>(object);
        csGameObject::destroy(gameObject);
    }
}

static std::set<void *> sObjectSet;

csGKObjectRef csGKObject::_checkout(void *ptr) {
    if (sObjectSet.find(ptr) != sObjectSet.end()) {
        return ((csGKObject *)ptr)->strongRef();
    } else {
        return nullptr;
    }
}

csGKObject::csGKObject () { sObjectSet.insert(this); }
csGKObject::~csGKObject() { sObjectSet.erase (this); }
