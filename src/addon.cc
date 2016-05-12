#define DEBUG

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <node.h>
#include "module/blake2b.hh"

using std::string;
using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void CreateHash(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	#ifdef DEBUG
	std::cout << "Called 'CreateHash' with " << args.Length() << " arguments!" << std::endl;
	#endif

	if (args.Length() < 1) { // if no algorithm passed
		isolate->ThrowException(Exception::SyntaxError(
			String::NewFromUtf8(isolate, "Algorithm not specified!")));
		return;
	}

	if (!args[0]->IsString()) {
		isolate->ThrowException(Exception::SyntaxError(
			String::NewFromUtf8(isolate, "Algorithm must be a string!")));
		return;
	}

	string algorithm = string(*String::Utf8Value(args[0]->ToString()));

	#ifdef DEBUG
	std::cout << "  arg0 = " << algorithm << std::endl;
	#endif

	if (algorithm == "blake2b") {
		Blake2b::NewInstance(args); //FIXME Generates error on call
		//TODO support for blake2bp, blake2s and blake2sp
	} else {
		isolate->ThrowException(Exception::ReferenceError(
			String::NewFromUtf8(isolate, "Algorithm not supported yet!")));
		return;
	}
}

void Init(Local<Object> exports, Local<Object> module) {
	// Init modules
	Blake2b::Init(exports->GetIsolate());
	//TODO init blake2bp, blake2s and blake2sp

	// Static `createHash` function
	NODE_SET_METHOD(exports, "createHash", CreateHash);

	#ifdef DEBUG
	std::cout << "Components succesfully initialized!" << std::endl;
	#endif
}

NODE_MODULE(addon, Init)
