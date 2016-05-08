#include "blake2b.hh"

// Constructor template
Persistent<Function> Blake2b::constructor;

// Private constructor/destructor
Blake2b::Blake2b() {};
Blake2b::~Blake2b() {};

// Initialize the class
void Blake2b::Init(Local<Object> exports) {
	Isolate* isolate = exports->GetIsolate();

	// Constructor Template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(String::NewFromUtf8(isolate, "Blake2b"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "update", Update);
	NODE_SET_PROTOTYPE_METHOD(tpl, "digest", Digest);
	//TODO NODE_SET_PROTOTYPE_METHOD(tpl, "name", Func);

	constructor.Reset(isolate, tpl->GetFunction());
}

// Wrappable Constructor
void Blake2b::New(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) {
		// Constructor called with `new`
		Blake2b* instance = new Blake2b();

		uint32_t outlen;
		switch (args.Length()) {
			case 1:
				if (args[0]->IsString())
		}
		if (args.Length() > 0) {
			if (args.Length() > 1 && args[1]->IsInt32()) {
				outlen = args[1]->Int32Value();
			}	else if (args[0]->IsInt32()) {
				outlen = args[0]->Int32Value();
			}
		} else {
			blake2b_init(&(instance->state), BLAKE2B_OUTBYTES);
		}

		//TODO blake2b_init

		instance->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	} else {
		// Constructor called as method
		Local<Function> constr = Local<Function>::New(isolate, constructor);
		args.GetReturnValue().Set(constr->NewInstance());
	}
}

// Object factory
void Blake2b::NewInstance(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	// Create instance
	Local<Function> constr = Local<Function>::New(isolate, constructor);
	Local<Object> instance = constr->NewInstance();

	args.GetReturnValue().Set(instance);
}


// API - Prototype
void Blake2b::Update(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	Blake2b* instance = ObjectWrap::Unwrap<Blake2b>(args.Holder());

	// arg0 (data) = string / Buffer
	// arg1 (encoding) = ([utf8], ascii, binary, hex) -optional

	if (args.Length() < 1) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "No data passed!"));
		return;
	}

	if (node::Buffer::HasInstance(args[0])) {
		// It is a buffer
		const uint8_t* data = (uint8_t*) node::Buffer::Data(args[0]);
		uint64_t length = node::Buffer::Length(args[0]);

		int res = blake2b_update(&(instance->state), data, length);
		args.GetReturnValue().Set(res);
	} else {
		isolate->ThrowException(String::NewFromUtf8(isolate, "String support is yet to be implemented!"));
	}
}

void Blake2b::Digest(const FunctionCallbackInfo<Value>& args) {

}
