#include <iostream>
#include <JavaScriptCore/JavaScriptCore.h>
#include <thread>

#include "./engine.h"
#include "utils.h"
#include "block_queue.h"

engine engineContext;
block_queue<std::function<void()>> taskQueue;

[[noreturn]]
void executeLoop() {
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