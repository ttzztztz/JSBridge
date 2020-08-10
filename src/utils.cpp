#include "utils.h"

JSObjectRef utils::make_object(JSContextRef ctx, const std::string &className,
                               const std::unordered_map<std::string, JSValueRef> &data) {
    JSClassDefinition definition = kJSClassDefinitionEmpty;
    definition.className = className.c_str();
    JSClassRef parentClass = JSClassCreate(&definition);
    JSObjectRef obj =
            JSObjectMake(ctx, parentClass, nullptr);

    for (auto &p : data) {
        JSObjectSetProperty(ctx, obj,
                            JSStringCreateWithUTF8CString(p.first.c_str()),
                            p.second,
                            kJSPropertyAttributeNone, nullptr);
    }

    return obj;
}

JSValueRef utils::evaluateScriptsFromFile(JSContextRef ctx, const std::string &fileName) {
    std::fstream fs(fileName.c_str());
    std::stringstream ss;
    ss << fs.rdbuf();
    std::string codes(ss.str());
    fs.close();
    ss.clear();

    return utils::evaluateScriptsFromString(ctx, codes);
}

JSObjectRef utils::generateJSBridgeObject(JSContextRef ctx) {
    auto obj = utils::make_object(ctx, "RabbitJSBridgeClass", std::unordered_map<std::string, JSValueRef>());
    utils::createMethodInObject(ctx, obj, "invoke", &utils::invoke);
    return obj;
}

JSValueRef utils::invoke(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount,
                         const JSValueRef *arguments, JSValueRef *exception) {
    const auto methodName = (JSStringRef) arguments[0];
    const auto args = (JSObjectRef) arguments[1];

    if (JSStringIsEqual(methodName, JSStringCreateWithUTF8CString("StdinSync"))) {
        std::string cb = methods::StdinSyncFunction(ctx, args);
        std::unordered_map<std::string, JSValueRef> res;

        JSStringRef data = JSStringCreateWithUTF8CString(cb.c_str());
        res["data"] = JSValueMakeString(ctx, data);
        JSObjectRef messageObject = utils::make_object(ctx, "Object", res);
        return messageObject;
    }

    return JSValueMakeUndefined(ctx);
}

void utils::callback(JSContextRef ctx, JSValueRef cbId, JSObjectRef data) {
//    JSValueRef callbackObjValue = JSObjectGetProperty(ctx, globalObject,
//                                           JSStringCreateWithUTF8CString("RabbitBridgeCallback")
//                                           , nullptr);
//
//    JSObjectRef callbackObj = JSValueToObject(ctx, callbackObjValue, nullptr);
//    if (JSObjectIsFunction(ctx, callbackObj)) {
//        JSValueRef arguments[] = {
//                cbId,
//                data
//        };
//        JSObjectCallAsFunction(ctx, callbackObj, nullptr, 2, arguments, nullptr);
//    }
}

JSObjectRef utils::generateConsoleObject(JSContextRef ctx) {
    auto log = [](JSContextRef ctx, JSObjectRef function, JSObjectRef this_object,
                   size_t argument_count, const JSValueRef arguments[],
                   JSValueRef *exception) -> JSValueRef {
        std::string logs;
        for (size_t i = 0; i < argument_count; i++) {
            auto str = utils::JSStringToStdString(ctx, arguments[i]);
            logs.append(str);
            if (i != argument_count - 1) logs.append(" ");
        }
        std::cout << "[LOG]" << logs << std::endl;
        return JSValueMakeUndefined(ctx);
    };

    auto error = [](JSContextRef ctx, JSObjectRef function, JSObjectRef this_object,
                     size_t argument_count, const JSValueRef arguments[],
                     JSValueRef *exception) -> JSValueRef {
        std::string logs;
        for (size_t i = 0; i < argument_count; i++) {
            auto str = utils::JSStringToStdString(ctx, arguments[i]);
            logs.append(str);
            if (i != argument_count - 1) logs.append(" ");
        }
        std::cerr << "[ERROR]" << logs << std::endl;
        return JSValueMakeUndefined(ctx);
    };

    auto warn = [](JSContextRef ctx, JSObjectRef function, JSObjectRef this_object,
                    size_t argument_count, const JSValueRef arguments[],
                    JSValueRef *exception) -> JSValueRef {
        std::string logs;
        for (size_t i = 0; i < argument_count; i++) {
            auto str = utils::JSStringToStdString(ctx, arguments[i]);
            logs.append(str);
            if (i != argument_count - 1) logs.append(" ");
        }
        std::cout << "[WARN]" << logs << std::endl;
        return JSValueMakeUndefined(ctx);
    };

    JSObjectRef obj = make_object(ctx, "Console", std::unordered_map<std::string, JSValueRef>());

    utils::createMethodInObject(ctx, obj, "log", log);
    utils::createMethodInObject(ctx, obj, "warn", warn);
    utils::createMethodInObject(ctx, obj, "error", error);

    return obj;
}

std::string utils::JSStringToStdString(JSContextRef ctx, JSValueRef value) {
    JSStringRef item = JSValueToStringCopy(ctx, value, nullptr);
    size_t str_len = JSStringGetLength(item);
    auto str = std::make_unique<char[]>(str_len + 1);
    JSStringGetUTF8CString(item, str.get(), str_len + 1);
    JSStringRelease(item);
    return std::string(str.get());
}

void utils::createMethodInObject(JSContextRef ctx,
                          JSObjectRef obj,
                          const std::string& name,
                          JSObjectCallAsFunctionCallback method) {
    JSStringRef function_name = JSStringCreateWithUTF8CString(name.c_str());
    JSObjectRef function_object =
            JSObjectMakeFunctionWithCallback(ctx, function_name, method);
    JSObjectSetProperty(ctx, obj, function_name, function_object,
                        kJSPropertyAttributeNone, nullptr);
    JSStringRelease(function_name);
}

JSValueRef utils::evaluateScriptsFromString(JSContextRef ctx, const std::string &codes) {
    JSStringRef statement = JSStringCreateWithUTF8CString(codes.c_str());
    JSValueRef res = JSEvaluateScript(ctx, statement,
                                      nullptr, nullptr,
                                      1, nullptr);

    JSStringRelease(statement);
    return res;
}
