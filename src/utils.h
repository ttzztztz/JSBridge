#ifndef JSCORE_UTILS_H
#define JSCORE_UTILS_H

#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <functional>
#include <JavaScriptCore/JavaScriptCore.h>

#include "methods.h"
#include "engine.h"
#include "block_queue.h"

extern engine engineContext;
extern block_queue<std::function<void()>> taskQueue;

class utils {
public:
    static JSObjectRef make_object(JSContextRef ctx, const std::string& className,
                                   const std::unordered_map<std::string, JSValueRef>& data);

    static JSValueRef evaluateScriptsFromFile(JSContextRef ctx, const std::string& fileName);

    static JSValueRef evaluateScriptsFromString(JSContextRef ctx, const std::string& codes);

    static JSObjectRef generateJSBridgeObject(JSContextRef ctx);

    static JSValueRef invoke(JSContextRef ctx, JSObjectRef function,
            JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef *exception);

    static void callback(JSContextRef ctx, JSValueRef cbId, JSObjectRef data);

    static void triggerEvent(JSContextRef ctx, const std::string& eventName, JSObjectRef data);

    static JSObjectRef generateConsoleObject(JSContextRef ctx);

    static std::string JSStringToStdString(JSContextRef ctx, JSValueRef value);

    static void createMethodInObject(JSContextRef ctx, JSObjectRef obj,
                                            const std::string& name,
                                            JSObjectCallAsFunctionCallback method);
};


#endif //JSCORE_UTILS_H
