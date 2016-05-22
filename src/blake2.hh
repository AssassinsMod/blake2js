#ifndef B2JS_BLAKE2_H
#define B2JS_BLAKE2_H

#include <cstring> // memcpy
#include <string>
#include <node.h>
#include <node_buffer.h>
#include <node_object_wrap.h>
#include "../lib/BLAKE2/sse/blake2.h"

namespace B2JS {

union blake2_state {
	blake2s_state  s;
	blake2sp_state sp;
	blake2b_state  b;
	blake2bp_state bp;
};

class Blake2 : public node::ObjectWrap {
public:
	static void Init(v8::Isolate* isolate);
	static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);

private:
	uint8_t outlen;
	blake2_state state;

	int (*update)(void*, const uint8_t*, uint64_t);
	int (*digest)(void*, uint8_t*, uint8_t);

	explicit Blake2(); ~Blake2();
	static v8::Persistent<v8::Function> constructor;
	static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void Update(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Digest(const v8::FunctionCallbackInfo<v8::Value>& args);
};

} // B2JS

#endif
