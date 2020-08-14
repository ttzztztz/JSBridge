#include "methods.h"

std::mutex iosMutex;

void methods::StdOutCore(const std::string &message) {
    const std::unique_lock<std::mutex> guard(iosMutex);
    std::cout << message << std::endl;
}

std::string methods::StdInCore() {
    const std::unique_lock<std::mutex> guard(iosMutex);
    std::string tmp;
    std::cin >> tmp;

    return std::move(tmp);
}

JSObjectRef methods::StdinSyncFunction(JSContextRef ctx, JSObjectRef args, const JSValueRef *arguments) {
    JSValueRef str = JSObjectGetProperty(ctx, args, *SafeJSString("message"), nullptr);

    std::string buf = utils::JSStringToStdString(ctx, str);

    methods::StdOutCore(buf);
    std::string tmp = methods::StdInCore();

    std::unordered_map<std::string, JSValueRef> res;
    SafeJSString data(tmp.c_str());
    res["data"] = JSValueMakeString(ctx, *data);
    JSObjectRef messageObject = utils::make_object(ctx, "Object", res);
    return messageObject;
}

JSObjectRef methods::StdinFunction(JSContextRef ctx, JSObjectRef args, const JSValueRef *arguments) {
    const auto callbackId = arguments[2];
    JSValueRef str = JSObjectGetProperty(ctx, args, *SafeJSString("message"), nullptr);
    std::string buf = utils::JSStringToStdString(ctx, str);
    methods::StdOutCore(buf);

    std::thread t([=]() -> void {
        std::string tmp = methods::StdInCore();

        std::unordered_map<std::string, JSValueRef> res;
        SafeJSString data(tmp.c_str());
        res["data"] = JSValueMakeString(ctx, *data);
        JSObjectRef messageObject = utils::make_object(ctx, "Object", res);

        utils::callback(ctx, callbackId, messageObject);
    });
    t.detach();

    return utils::make_object(ctx, "Object", std::unordered_map<std::string, JSValueRef>());
}

JSObjectRef methods::ReadFileSyncFunction(JSContextRef ctx, JSObjectRef args, const JSValueRef *arguments) {
    JSValueRef str = JSObjectGetProperty(ctx, args, *SafeJSString("path"), nullptr);

    std::string buf = "resources/" + utils::JSStringToStdString(ctx, str);
    std::fstream fs(buf.c_str());
    if (fs.is_open()) {
        std::stringstream ss;
        ss << fs.rdbuf();
        fs.close();
        size_t contentLen = ss.str().size();

        char* content = new char[contentLen];
        memcpy(content, ss.str().c_str(), contentLen);

        std::unordered_map<std::string, JSValueRef> res;
        // https://github.com/WebKit/webkit/blob/950143da027e80924b4bb86defa8a3f21fd3fb1e/Source/JavaScriptCore/API/JSTypedArray.cpp#L172
        res["data"] = JSObjectMakeArrayBufferWithBytesNoCopy(
                ctx, reinterpret_cast<void*>(content), contentLen,
                [](void* ptr, void* _ctx) -> void {
                    std::cout << "removed " << std::endl;
                    delete[] reinterpret_cast<char*>(ptr);
                }, nullptr, nullptr);

        JSObjectRef messageObject = utils::make_object(ctx, "Object", res);
        return messageObject;
    }

    std::unordered_map<std::string, JSValueRef> res;
    res["data"] = JSValueMakeUndefined(ctx);
    JSObjectRef messageObject = utils::make_object(ctx, "Object", res);
    return messageObject;
}

JSObjectRef methods::launchEvent(JSContextRef ctx, time_t timestamp) {
    std::unordered_map<std::string, JSValueRef> res;
    res["timestamp"] = JSValueMakeNumber(ctx, timestamp);
    JSObjectRef messageObject = utils::make_object(ctx, "Object", res);

    return messageObject;
}

JSObjectRef methods::ReadFileFunction(JSContextRef ctx, JSObjectRef args, const JSValueRef *arguments) {
    const auto callbackId = arguments[2];
    JSValueRef _path = JSObjectGetProperty(ctx, args, *SafeJSString("path"), nullptr);
    std::string path = utils::JSStringToStdString(ctx, _path);

    std::thread t([=]() -> void {
        std::string buf = "resources/" + path;
        std::fstream fs(buf.c_str());
        if (fs.is_open()) {
            std::stringstream ss;
            ss << fs.rdbuf();
            fs.close();
            size_t contentLen = ss.str().size();

            char* content = new char[contentLen];
            memcpy(content, ss.str().c_str(), contentLen);

            std::unordered_map<std::string, JSValueRef> res;
            // https://github.com/WebKit/webkit/blob/950143da027e80924b4bb86defa8a3f21fd3fb1e/Source/JavaScriptCore/API/JSTypedArray.cpp#L172
            res["data"] = JSObjectMakeArrayBufferWithBytesNoCopy(
                    ctx, reinterpret_cast<void*>(content), contentLen,
                    [](void* ptr, void* _ctx) -> void {
                        std::cout << "removed " << std::endl;
                        delete[] reinterpret_cast<char*>(ptr);
                    }, nullptr, nullptr);

            JSObjectRef messageObject = utils::make_object(ctx, "Object", res);
            utils::callback(ctx, callbackId, messageObject);
        }

        std::unordered_map<std::string, JSValueRef> res;
        res["data"] = JSValueMakeUndefined(ctx);
        JSObjectRef messageObject = utils::make_object(ctx, "Object", res);
        utils::callback(ctx, callbackId, messageObject);
    });
    t.detach();

    return utils::make_object(ctx, "Object", std::unordered_map<std::string, JSValueRef>());
}
