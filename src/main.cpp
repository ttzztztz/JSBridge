#include <iostream>
#include <JavaScriptCore/JavaScriptCore.h>
#include <thread>

#include "engine.h"
#include "utils.h"
#include "block_queue.h"
#include "SafeJSString.h"

engine engineContext;
block_queue<std::function<void()>> taskQueue;

[[noreturn]]
void executeLoop() {
    JSObjectSetProperty(engineContext.globalContext, engineContext.globalObject,
                        *SafeJSString("RabbitJSBridge"),
                        utils::generateJSBridgeObject(engineContext.globalContext),
                        kJSPropertyAttributeNone, nullptr);

    JSObjectSetProperty(engineContext.globalContext, engineContext.globalObject,
                        *SafeJSString("console"),
                        utils::generateConsoleObject(engineContext.globalContext),
                        kJSPropertyAttributeNone, nullptr);


    // js bridge inject
    utils::evaluateScriptsFromFile(engineContext.globalContext,
                                   "./javascript/jsbridge.js");

    // lib inject
    utils::evaluateScriptsFromFile(engineContext.globalContext,
                                   "./javascript/lib.js");

    utils::triggerEvent(engineContext.globalContext,
                        "onLaunch",
                        methods::launchEvent(engineContext.globalContext, time(nullptr)));


    taskQueue.push([&]() -> void {
        utils::evaluateScriptsFromFile(engineContext.globalContext,
                                       "./javascript/main.js");

        JSGarbageCollect(engineContext.globalContext);
    });

    while (true) {
        std::function<void()> func = taskQueue.pop();
        func();
    }
}

int main() {
    std::thread executeThread([&]() -> void {
        executeLoop();
    });
    executeThread.join();

    return 0;
}