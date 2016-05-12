#ifndef JSB2_UTILS_H
#define JSB2_UTILS_H

#include <string>
#include <node.h>
#include <node_buffer.h>
#include "../lib/BLAKE2/sse/blake2.h"

namespace B2JS {

class Utils {
public:
	static void ParseOptions(v8::Isolate* isolate, v8::Local<v8::Value> arg, blake2b_constant max_keylen, std::string& key, uint8_t& outlen);
};

}

#endif
