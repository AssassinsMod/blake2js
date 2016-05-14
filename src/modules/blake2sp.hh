#ifndef B2JS_BLAKE2SP_H
#define B2JS_BLAKE2SP_H

#include <cstring> // memcpy
#include <string>
#include <node.h>
#include <node_buffer.h>
#include <node_object_wrap.h>
#include "../../lib/BLAKE2/sse/blake2.h"
#include "../utils.hh"

namespace B2JS {

class Blake2sp : public node::ObjectWrap {
public:
	static void Init(v8::Isolate* isolate);
	static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);

private:
	uint8_t outlen;
	blake2sp_state state;

	explicit Blake2sp(uint8_t length = BLAKE2S_OUTBYTES, std::string key = "");
	 ~Blake2sp();
	static v8::Persistent<v8::Function> constructor;
	static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void Update(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void Digest(const v8::FunctionCallbackInfo<v8::Value>& args);
};

} // B2JS

#endif
