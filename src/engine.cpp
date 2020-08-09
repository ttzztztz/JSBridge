#include "engine.h"

#include <iostream>

engine::engine() {
    this->group = JSContextGroupCreate();
    this->globalContext = JSGlobalContextCreateInGroup(this->group, nullptr);
    this->globalObject = JSContextGetGlobalObject(this->globalContext);
}

engine::~engine() {
    JSGlobalContextRelease(this->globalContext);
    JSContextGroupRelease(this->group);

    std::cout << "released" << std::endl;
}
