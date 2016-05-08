#include <cstdint>
#include <cstdlib>
#include <string>

#include <node.h>

#include "module/blake2b.hh"

using namespace v8;
using std::string;

void CreateHash(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	if (args.Length() > 0) {
		if (!args[0]->IsString()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "First arg must be a string!")));
			return;
		}

		string arg0 = string(*(String::Utf8Value(args[0]->ToString())));

		/* When the `NewInstance` method is called, the arguments along with the
		 * function context are passed as `args`. Then the new instance of the
		 * specific class is wrapped around the caller's `this`, so this method
		 * doesn't really need to return a value.                              */
		if (arg0 == "blake2b") {
			Blake2b::NewInstance(args);
		} else if (arg0 == "blake2bp") {

		} else if (arg0 == "blake2s") {

		} else if (arg0 == "blake2sp") {

		} else {
			isolate->ThrowException(Exception::SyntaxError(
				String::NewFromUtf8(isolate, "Unsupported algorithm")));
			return;
		}
	}
	//TODO create object

	args.GetReturnValue().Set(String::NewFromUtf8(isolate, "test"));
}

void Init(Local<Object> exports, Local<Object> module) {
	// Init modules
	Blake2b::Init(exports);

	// Static `createHash` function
	NODE_SET_METHOD(exports, "createHash", CreateHash);
}

NODE_MODULE(blake2, Init)
