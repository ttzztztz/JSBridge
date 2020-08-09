#ifndef JSCORE_UTILS_H
#define JSCORE_UTILS_H

#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <JavaScriptCore/JavaScriptCore.h>

#include "methods.h"

class utils {
public:
    static JSObjectRef make_object(JSContextRef ctx, const std::string& className,
                                   const std::unordered_map<std::string, JSValueRef>& data);

    static void evaluateScripts(JSContextRef ctx, const std::string& fileName);

    static JSObjectRef generateJSBridgeObject(JSContextRef ctx);

    static JSValueRef invoke(JSContextRef ctx, JSObjectRef function,
            JSObjectRef thisObject, size_t argumentCount,
            const JSValueRef arguments[], JSValueRef *exception);

    static void callback(JSValueRef cbId, JSObjectRef data);

    static JSObjectRef generateConsoleObject(JSContextRef ctx);

    static std::string JSStringToStdString(JSContextRef ctx, JSValueRef value);

    static void utils::createMethodInObject(JSContextRef ctx, JSObjectRef target_object,
                                            const std::string& name,
                                            JSObjectCallAsFunctionCallback method)
};


#endif //JSCORE_UTILS_H
