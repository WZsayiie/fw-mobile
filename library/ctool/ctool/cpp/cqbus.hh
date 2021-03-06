#pragma once

#include "cqctoolbase.hh"

_CQCTOOL_BEGIN_VERSION_NS

typedef const char *cqBusEventName;

struct cqBusEvent {
    cqBusEventName name = nullptr;
    cqObjectRef parameter;
};

typedef void (*cqBusConsumer)(cqObjectRef observer, cqBusEvent event);

cq_class(cqBus, cqObject) {
    
    //a cqBus object can be called on different threads.
    
    static cqBusRef get();
    
    //cqBus stores weak reference, call removeObserver() isn't required.
    virtual void addObserver(
        cqObjectRef observer, cqBusEventName eventName, cqBusConsumer consumer);
    
    //if $eventName is nullptr, remove all event listening of $observer.
    virtual void removeObserver(cqObjectRef observer, cqBusEventName eventName);
    
    virtual void post(cqBusEventName eventName, cqObjectRef parameter);
};

_CQCTOOL_END_VERSION_NS
