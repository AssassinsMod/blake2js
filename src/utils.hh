#ifndef B2JS_UTILS_H
#define B2JS_UTILS_H

#include <string>
#include <node.h>
#include <node_buffer.h>

namespace B2JS {

class Utils {
public:
	static void ParseOptions(v8::Isolate* isolate, v8::Local<v8::Value> arg, uint8_t max_keylen, std::string& key, uint8_t& outlen);
	static void PrintHex(const uint8_t* data, size_t length);
};

}

#endif
