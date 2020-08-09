#include <iostream>
#include <JavaScriptCore/JavaScriptCore.h>

JSValueRef HelloCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount,
                         const JSValueRef arguments[], JSValueRef *exception) {
    std::cout << "Hello World" << std::endl;
    return JSValueMakeUndefined(ctx);
}

int main(int argc, const char *argv[]) {
    JSContextGroupRef group = JSContextGroupCreate();
    JSGlobalContextRef globalContext = JSGlobalContextCreateInGroup(group, nullptr);
    JSObjectRef globalObject = JSContextGetGlobalObject(globalContext);
    JSStringRef helloFunctionName = JSStringCreateWithUTF8CString("hello");

    JSObjectRef functionObject = JSObjectMakeFunctionWithCallback(globalContext, helloFunctionName, &HelloCallback);
    JSObjectSetProperty(globalContext, globalObject, helloFunctionName, functionObject, kJSPropertyAttributeNone,
                        nullptr);

    JSStringRef statement = JSStringCreateWithUTF8CString("hello()");
    JSValueRef rtn = JSEvaluateScript(globalContext, statement, nullptr, nullptr, 1, nullptr);

    const bool isUndefined = JSValueIsUndefined(globalContext, rtn);
    std::cout << isUndefined << std::endl;

    JSGlobalContextRelease(globalContext);
    JSContextGroupRelease(group);
    JSStringRelease(helloFunctionName);
    JSStringRelease(statement);

    return 0;
}