#include <string>
#include <node.h>
#include "blake2.hh"

namespace B2JS {

using std::string;
using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Undefined;
using v8::Value;

void Init(Local<Object> exports);
void CreateHash(const FunctionCallbackInfo<Value>& args);
void CreateHmac(const FunctionCallbackInfo<Value>& args);

NODE_MODULE(addon, Init)


// ================ ]  DEFINITIONS  [ ================ //
void Init(Local<Object> exports) {
	Blake2::Init(exports->GetIsolate());

	// Register Methods
	NODE_SET_METHOD(exports, "createHash", CreateHash);
	NODE_SET_METHOD(exports, "createHmac", CreateHmac);
}

// (CreateHash|CreateHmac): (algorithm, key, length)
void CreateHash(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	if (args[0]->IsUndefined() || !args[0]->IsString()) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Invalid or unspecified algorithm!"));
		return;
	}

	// If length is specified move it in position 2
	if (!args[1]->IsUndefined()) {
		args[2] = args[1];
		args[1] = Undefined(isolate);
	}

	Blake2::NewInstance(args);
}

void CreateHmac(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	if (args[0]->IsUndefined() || !args[0]->IsString()) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Invalid or unspecified algorithm!"));
		return;
	}

	if (args[1]->IsUndefined() || !args[1]->IsString()) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Invalid or unspecified key!"));
		return;
	}

	Blake2::NewInstance(args);
}

} // B2JS
