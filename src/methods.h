#ifndef JSCORE_METHODS_H
#define JSCORE_METHODS_H

#include <string>
#include <iostream>
#include <mutex>
#include <thread>

#include <JavaScriptCore/JavaScriptCore.h>

#include "utils.h"
#include "SafeJSString.h"

class methods {
public:
    static JSObjectRef StdinSyncFunction(JSContextRef ctx, JSObjectRef args, const JSValueRef *arguments);
    static JSObjectRef StdinFunction(JSContextRef ctx, JSObjectRef args, const JSValueRef *arguments);
    static JSObjectRef ReadFileSyncFunction(JSContextRef ctx, JSObjectRef args, const JSValueRef *arguments);
    static JSObjectRef ReadFileFunction(JSContextRef ctx, JSObjectRef args, const JSValueRef *arguments);

    static JSObjectRef launchEvent(JSContextRef ctx, time_t timestamp);
private:
    static void StdOutCore(const std::string& message);
    static std::string StdInCore();
};


#endif //JSCORE_METHODS_H
