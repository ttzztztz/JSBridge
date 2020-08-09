#ifndef JSCORE_ENGINE_H
#define JSCORE_ENGINE_H

#include <JavaScriptCore/JavaScriptCore.h>

class engine {
public:
    JSContextGroupRef group;
    JSGlobalContextRef globalContext;
    JSObjectRef globalObject;

    engine();
    ~engine();
};


#endif //JSCORE_ENGINE_H
