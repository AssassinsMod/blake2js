#include <string>
#include <node.h>
#include "modules/blake2s.hh"

namespace B2JS {

using std::string;
using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void Init(Local<Object> exports);
void CreateHash(const FunctionCallbackInfo<Value>& args);

NODE_MODULE(addon, Init)


// ================ ]  DEFINITIONS  [ ================ //
void Init(Local<Object> exports) {
	// Initialize modules
	Blake2s::Init(exports->GetIsolate());
	//TODO Blake2sp::Init(exports->GetIsolate());
	//TODO Blake2b::Init(exports->GetIsolate());
	//TODO Blake2bp::Init(exports->GetIsolate());

	// Register Methods
	NODE_SET_METHOD(exports, "createHash", CreateHash);
}

void CreateHash(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	if (args[0]->IsUndefined() || !args[0]->IsString()) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Invalid algorithm"));
		return;
	}

	string algorithm = string(*String::Utf8Value(args[0]->ToString()));

	if (algorithm == "blake2s") {
		Blake2s::NewInstance(args);
	} else { //TODO support other algorithms
		isolate->ThrowException(String::NewFromUtf8(isolate, "Algorithm not supported yet!"));
		return;
	}
}

} // B2JS
