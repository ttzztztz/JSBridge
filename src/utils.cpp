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

void utils::evaluateScripts(JSContextRef ctx, const std::string &fileName) {
    std::fstream fs(fileName.c_str());
    std::stringstream ss;
    ss << fs.rdbuf();
    std::string jsBridgeCodes(ss.str());
    fs.close();
    ss.clear();

    JSStringRef statement = JSStringCreateWithUTF8CString(jsBridgeCodes.c_str());
    JSEvaluateScript(ctx, statement,
                     nullptr, nullptr,
                     1, nullptr);

    JSStringRelease(statement);
}

JSObjectRef utils::generateJSBridgeObject(JSContextRef ctx) {
    std::unordered_map<std::string, JSValueRef> objs;
    JSStringRef invokeFunctionName = JSStringCreateWithUTF8CString("invoke");
    JSObjectRef invokeObject =
            JSObjectMakeFunctionWithCallback(ctx, invokeFunctionName, &utils::invoke);
    objs["invoke"] = invokeObject;

    return utils::make_object(ctx, "RabbitJSBridgeClass", objs);
}

JSValueRef utils::invoke(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount,
                         const JSValueRef *arguments, JSValueRef *exception) {
    const auto methodName = (JSStringRef) arguments[0];
    const auto args = (JSObjectRef) arguments[1];

    if (JSStringIsEqual(methodName, JSStringCreateWithUTF8CString("Stdin"))) {
        std::string cb = methods::StdinFunction(ctx, args);
        if (JSValueIsNumber(ctx, arguments[2])) {
            std::unordered_map<std::string, JSValueRef> res;
            res["data"] = (JSValueRef) JSStringCreateWithUTF8CString(cb.c_str());
            JSObjectRef messageObject = utils::make_object(ctx, "messageObject", res);
            callback(arguments[2], messageObject);
        }
    }

    return JSValueMakeUndefined(ctx);
}

void utils::callback(JSValueRef cbId, JSObjectRef data) {

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
                          JSObjectRef target_object,
                          const std::string& name,
                          JSObjectCallAsFunctionCallback method) {
    JSStringRef function_name = JSStringCreateWithUTF8CString(name.c_str());
    JSObjectRef function_object =
            JSObjectMakeFunctionWithCallback(ctx, function_name, method);
    JSObjectSetProperty(ctx, target_object, function_name, function_object,
                        kJSPropertyAttributeNone, nullptr);
    JSStringRelease(function_name);
}
