#ifndef JSCORE_METHODS_H
#define JSCORE_METHODS_H

#include <string>
#include <iostream>
#include <JavaScriptCore/JavaScriptCore.h>

#include "utils.h"

class methods {
public:
    static std::string StdinFunction(JSContextRef ctx, JSObjectRef args);
};


#endif //JSCORE_METHODS_H
