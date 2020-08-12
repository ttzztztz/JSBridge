#ifndef JSCORE_SAFEJSSTRING_H
#define JSCORE_SAFEJSSTRING_H

#include <iostream>
#include <JavaScriptCore/JavaScriptCore.h>

class SafeJSString {
public:
    explicit SafeJSString(const std::string& str) {
        this->ref = JSStringCreateWithUTF8CString(str.c_str());
    }
    explicit SafeJSString(const char* str) {
        this->ref = JSStringCreateWithUTF8CString(str);
    }

    SafeJSString(SafeJSString&& v) = delete;
    SafeJSString(SafeJSString& v) = delete;

    ~SafeJSString() {
        JSStringRelease(this->ref);
    }

    JSStringRef operator*() const {
        return this->ref;
    }
private:
    JSStringRef ref;
};


#endif //JSCORE_SAFEJSSTRING_H
