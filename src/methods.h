#ifndef JSCORE_METHODS_H
#define JSCORE_METHODS_H

#include <string>
#include <iostream>
#include <mutex>
#include <JavaScriptCore/JavaScriptCore.h>

#include "utils.h"

class methods {
public:
    static JSObjectRef StdinSyncFunction(JSContextRef ctx, JSObjectRef args);
    static JSObjectRef ReadFileFunction(JSContextRef ctx, JSObjectRef args);
};


#endif //JSCORE_METHODS_H
