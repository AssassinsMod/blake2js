#define DEBUG

#include <iostream>
#include <string>
#include <node.h>
#include "modules/blake2s.hh"
#include "modules/blake2b.hh"

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

	#ifdef DEBUG
	std::cout << "Components succesfully initialized!" << std::endl;
	#endif
}

void CreateHash(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	#ifdef DEBUG
	std::cout << "Called 'CreateHash' with " << args.Length() << " arguments!" << std::endl << "?1 = " << (args[1]->IsUndefined() ? "y" : "n") << std::endl;
	#endif

	if (args[0]->IsUndefined() || !args[0]->IsString()) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Invalid algorithm"));
		return;
	}

	string algorithm = string(*String::Utf8Value(args[0]->ToString()));

	#ifdef DEBUG
	std::cout << "  arg0 = " << algorithm << std::endl;
	#endif

	if (algorithm == "blake2s") {
		Blake2s::NewInstance(args);
	} else if (algorithm == "blake2b") {
		Blake2b::NewInstance(args);
	} else { //TODO support other algorithms
		isolate->ThrowException(String::NewFromUtf8(isolate, "Algorithm not supported yet!"));
		return;
	}
}

} // B2JS
