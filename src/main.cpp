#include <iostream>
#include <JavaScriptCore/JavaScriptCore.h>
#include <memory>

#include "./engine.h"
#include "utils.h"

int main(int argc, const char *argv[]) {
    std::shared_ptr<engine> ctx(new engine());

    JSObjectSetProperty(ctx->globalContext, ctx->globalObject,
                        JSStringCreateWithUTF8CString("RabbitJSBridge"),
                        utils::generateJSBridgeObject(ctx->globalContext),
                        kJSPropertyAttributeNone, nullptr);

    JSObjectSetProperty(ctx->globalContext, ctx->globalObject,
                        JSStringCreateWithUTF8CString("console"),
                        utils::generateConsoleObject(ctx->globalContext),
                        kJSPropertyAttributeNone, nullptr);

    // js bridge
    utils::evaluateScripts(ctx->globalContext, "./javascript/jsbridge.js");

    // main codes
    utils::evaluateScripts(ctx->globalContext, "./javascript/main.js");

    return 0;
}