#include "methods.h"

std::string methods::StdinSyncFunction(JSContextRef ctx, JSObjectRef args) {
    JSValueRef str = JSObjectGetProperty(ctx, args, JSStringCreateWithUTF8CString("message"), nullptr);

    std::string buf = utils::JSStringToStdString(ctx, str);
    std::cout << buf << std::endl;

    std::string tmp;
    std::cin >> tmp;
    return tmp;
}
