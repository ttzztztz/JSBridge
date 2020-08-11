#include <iostream>
#include <JavaScriptCore/JavaScriptCore.h>
#include <thread>

#include "./engine.h"
#include "utils.h"
#include "block_queue.h"

[[noreturn]]
void executeLoop(block_queue<std::string>& taskQueue) {
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

    while (true) {
        std::string scripts = std::move(taskQueue.pop());
        utils::evaluateScriptsFromString(engineContext.globalContext,
                                         scripts);
    }
}

int main() {
    block_queue<std::string> taskQueue;
    std::thread executeThread([&]() -> void {
        executeLoop(taskQueue);
    });
    executeThread.join();
    
    return 0;
}