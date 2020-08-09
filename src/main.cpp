#include <iostream>
#include <JavaScriptCore/JavaScriptCore.h>

#include "./engine.h"
#include "utils.h"

int main(int argc, const char *argv[]) {
    engine engineContext;

    JSObjectSetProperty(engineContext.globalContext, engineContext.globalObject,
                        JSStringCreateWithUTF8CString("RabbitJSBridge"),
                        utils::generateJSBridgeObject(engineContext.globalContext),
                        kJSPropertyAttributeNone, nullptr);

    JSObjectSetProperty(engineContext.globalContext, engineContext.globalObject,
                        JSStringCreateWithUTF8CString("console"),
                        utils::generateConsoleObject(engineContext.globalContext),
                        kJSPropertyAttributeNone, nullptr);


    // js bridge
    utils::evaluateScriptsFromFile(engineContext.globalContext,
                                   "./javascript/jsbridge.js");

    // main codes
    utils::evaluateScriptsFromFile(engineContext.globalContext,
                                   "./javascript/main.js");

    return 0;
}