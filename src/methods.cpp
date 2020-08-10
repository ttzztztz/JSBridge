#include "methods.h"

JSObjectRef methods::StdinSyncFunction(JSContextRef ctx, JSObjectRef args) {
    JSValueRef str = JSObjectGetProperty(ctx, args, JSStringCreateWithUTF8CString("message"), nullptr);

    std::string buf = utils::JSStringToStdString(ctx, str);
    std::cout << buf << std::endl;

    std::string tmp;
    std::cin >> tmp;

    std::unordered_map<std::string, JSValueRef> res;
    JSStringRef data = JSStringCreateWithUTF8CString(tmp.c_str());
    res["data"] = JSValueMakeString(ctx, data);
    JSObjectRef messageObject = utils::make_object(ctx, "Object", res);
    return messageObject;
}

JSObjectRef methods::ReadFileFunction(JSContextRef ctx, JSObjectRef args) {
    JSValueRef str = JSObjectGetProperty(ctx, args, JSStringCreateWithUTF8CString("message"), nullptr);

    std::string buf = "resources/" + utils::JSStringToStdString(ctx, str);

    std::fstream fs(buf.c_str());
    if (fs.is_open()) {
        std::stringstream ss;
        ss << fs.rdbuf();
        fs.close();
        std::string all(ss.str());

        std::unordered_map<std::string, JSValueRef> res;
        JSStringRef data = JSStringCreateWithUTF8CString(all.c_str());
        res["data"] = JSValueMakeString(ctx, data);
        JSObjectRef messageObject = utils::make_object(ctx, "Object", res);
        return messageObject;
    }

    std::unordered_map<std::string, JSValueRef> res;
    res["data"] = JSValueMakeUndefined(ctx);
    JSObjectRef messageObject = utils::make_object(ctx, "Object", res);
    return messageObject;
}
