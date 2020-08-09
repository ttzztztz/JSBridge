#include <iostream>
#include <JavaScriptCore/JavaScriptCore.h>
#include <memory>

#include "./engine.h"

JSValueRef HelloCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount,
                         const JSValueRef arguments[], JSValueRef *exception) {
    std::cout << "Hello World" << std::endl;
    return JSValueMakeString(ctx, JSStringCreateWithUTF8CString("hzytql"));
}

int main(int argc, const char *argv[]) {
    std::shared_ptr<engine> ctx(new engine());

    JSStringRef helloFunctionName = JSStringCreateWithUTF8CString("hello");
    JSObjectRef functionObject =
            JSObjectMakeFunctionWithCallback(ctx->globalContext,
                                             helloFunctionName, &HelloCallback);

    JSObjectSetProperty(ctx->globalContext, ctx->globalObject,
                        helloFunctionName, functionObject, kJSPropertyAttributeNone,
                        nullptr);

    JSStringRef statement = JSStringCreateWithUTF8CString("hello()");
    JSValueRef rtn = JSEvaluateScript(ctx->globalContext, statement,
                                      nullptr, nullptr,
                                      1, nullptr);

    JSStringRelease(helloFunctionName);
    JSStringRelease(statement);

    if (JSValueIsString(ctx->globalContext, rtn)) {
        char buf[500];
        memset(buf, 0, sizeof buf);

        JSStringGetUTF8CString((JSStringRef) rtn, buf, sizeof buf);
        std::cout << buf << std::endl;
        JSStringRelease((JSStringRef) rtn);
    }

    return 0;
}