#include "../utils.hh"
#include "blake2b.hh"

using std::string;
using v8::Exception;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::MaybeLocal;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;

// Constructor template
Persistent<Function> Blake2b::constructor;

// Private constructor/destructor
Blake2b::Blake2b() {}
Blake2b::~Blake2b() {};

// Initialize the class
void Blake2b::Init(Isolate* isolate) {
	// Constructor Template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(String::NewFromUtf8(isolate, "Blake2b"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "update", Update);
	NODE_SET_PROTOTYPE_METHOD(tpl, "digest", Digest);

	constructor.Reset(isolate, tpl->GetFunction());
}

// Wrappable Constructor
void Blake2b::New(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	#ifdef DEBUG
	std::cout << "Blake2b wrapped constructor called." << std::endl;
	#endif

	if (args.IsConstructCall()) { // new MyObj(...);
		#ifdef DEBUG
		std::cout << "Blake2b constructor invoked with " << args.Length() << " arguments!" << std::endl;
		#endif

		// Parse args
		uint8_t len = BLAKE2B_OUTBYTES;
		string key;

		if (args.Length() > 0) { // If present parse options
			B2JS::Utils::ParseOptions(isolate, args[0], BLAKE2B_KEYBYTES, key, len);
		}

		// Create instance
		Blake2b* instance = new Blake2b();

		// Init instance state
		int res;
		if (key.empty()) {
			res = blake2b_init(instance->state, len);
		} else {
			res = blake2b_init_key(instance->state, len, key.c_str(), key.size());
		}

		// Check for blake2b_init internal errors
		if (res == 0) {
			instance->outlen = len;
		} else {
			isolate->ThrowException(Exception::Error(
				String::NewFromUtf8(isolate, "Internal initialization error!")));
			return;
		}

		// Return instance
		instance->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	} else { // MyObj(...);
		#ifdef DEBUG
		std::cout << "Blake2b constructor invoked as method with " << args.Length() << " arguments!" << std::endl;
		#endif

//		Blake2b::NewInstance(args); //FIXME not sure if this works correctly
	}
}

// Object factory
void Blake2b::NewInstance(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	#ifdef DEBUG
	std::cout << "test" << std::endl;
	std::cout << "Creating new Blake2b instance (" << args.Length() << " args passed)" << std::endl;
	#endif

	// Instantiate constructor
	Local<Function> constr = Local<Function>::New(isolate, constructor);
	Local<Object> instance;

	if (args.Length() > 1 && args[1]->IsObject()) { // With options
		Local<Value> options = args[1];
		instance = constr->NewInstance(1, &options);
	} else { // Default
		instance = constr->NewInstance();
	}

	// Return new instance
	args.GetReturnValue().Set(instance);
}


// API - Prototype
void Blake2b::Update(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	Blake2b* instance = ObjectWrap::Unwrap<Blake2b>(args.Holder());

	if (args.Length() > 0) {
		if (node::Buffer::HasInstance(args[0])) {
			Local<Object> buffer = args[0]->ToObject();

			char* data = node::Buffer::Data(buffer);
			size_t len = node::Buffer::Length(buffer);

			int res = blake2b_update(instance->state, (uint8_t*)data, len);

			if (res != 0) {
				isolate->ThrowException(Exception::Error(
					String::NewFromUtf8(isolate, "Internal update error!")));
				return;
			}
		} else if (args[0]->IsString()) {
			//TODO Add string input support
			isolate->ThrowException(Exception::ReferenceError(
				String::NewFromUtf8(isolate, "Strings not supported yet!")));
			return;
		} else {
			isolate->ThrowException(Exception::SyntaxError(
				String::NewFromUtf8(isolate, "Unsupported data format!")));
			return;
		}
	} else {
		isolate->ThrowException(Exception::SyntaxError(
			String::NewFromUtf8(isolate, "No data passed to the function!")));
		return;
	}
}

void Blake2b::Digest(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	Blake2b* instance = ObjectWrap::Unwrap<Blake2b>(args.Holder());

	uint8_t hash[instance->outlen];

	int res = blake2b_final(instance->state, hash, instance->outlen);

	if (res != 0) {
		isolate->ThrowException(Exception::Error(
			String::NewFromUtf8(isolate, "Internal final error!")));
		return;
	}

	Local<Object> buff = node::Buffer::New(isolate, (char*)hash, instance->outlen).ToLocalChecked();

	args.GetReturnValue().Set(buff);
}
