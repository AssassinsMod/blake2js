#include "blake2sp.hh"

namespace B2JS {

using std::string;
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

Blake2sp::Blake2sp(uint8_t length, string key) : outlen(length) {
	if (key.empty()) {
		blake2sp_init(&(this->state), this->outlen);
	} else {
		blake2sp_init_key(&(this->state), this->outlen, key.c_str(), (const uint8_t)key.length());
	}
}
Blake2sp::~Blake2sp() {}

Persistent<Function> Blake2sp::constructor;

void Blake2sp::Init(Isolate *isolate) {
	// Prepare template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(String::NewFromUtf8(isolate, "Blake2sp"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Set prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "update", Update);
	NODE_SET_PROTOTYPE_METHOD(tpl, "digest", Digest);

	// Prepare constructor
	constructor.Reset(isolate, tpl->GetFunction());
}

void Blake2sp::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) {
		// Instantiate class
		string key;
		uint8_t len = BLAKE2S_OUTBYTES;

		if (!args[0]->IsUndefined()) {
			Utils::ParseOptions(isolate, args[0], BLAKE2S_KEYBYTES, key, len);
		}

		Blake2sp* instance = new Blake2sp(len, key);

		// Wrap and return instance
		instance->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	} else {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Invalid instantiation!"));
		return;
	}
}

void Blake2sp::NewInstance(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	// Load constructor
	Local<Function> constr = Local<Function>::New(isolate, constructor);

	// Prepare and return instance
	if (args[1]->IsUndefined()) {
		args.GetReturnValue().Set(constr->NewInstance());
	} else {
		const unsigned argc = 1;
		Local<Value> argv = args[1];
		args.GetReturnValue().Set(constr->NewInstance(argc, &argv));
	}
}

// ================ ]  PROTOTYPE  [ ================ //
void Blake2sp::Update(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	Blake2sp* instance = ObjectWrap::Unwrap<Blake2sp>(args.Holder());

	// Check if data has been passed
	if (args[0]->IsUndefined()) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "No data passed for update!"));
		return;
	}

	if (node::Buffer::HasInstance(args[0])) { // if data is a buffer
		// Get data from buffer
		uint8_t*   data = (uint8_t*) node::Buffer::Data(args[0]);
		uint64_t length = (uint64_t) node::Buffer::Length(args[0]);

		// Perform update
		if (blake2sp_update(&(instance->state), data, length) != 0) {
			isolate->ThrowException(String::NewFromUtf8(isolate, "Internal blake2 error!"));
			return;
		}
	} else if (args[0]->IsString()) { // if data is a string
		isolate->ThrowException(String::NewFromUtf8(isolate, "String support yet to be implemented!"));
		return;
	} else {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Unsupported data format!"));
		return;
	}
}

void Blake2sp::Digest(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	Blake2sp* instance = ObjectWrap::Unwrap<Blake2sp>(args.Holder());

	// Create output buffer of the right length
	uint8_t hash[instance->outlen];

	// Perform digest
	if (blake2sp_final(&(instance->state), hash, instance->outlen) != 0) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Internal blake2 error!"));
		return;
	}

	// Create buffer and copy data into it
	Local<Object> res = node::Buffer::New(isolate, sizeof(hash)).ToLocalChecked();
	memcpy(node::Buffer::Data(res), (char*)hash, sizeof(hash));
	/* FIXME node::Buffer::New with data crashes.
	 * For some reason node::Buffer::New(isolate, (char*)hash, sizeof(hash));
	 * doesn't work, so for now we'll manually copy the data inside the newly
	 * created buffer. */

	args.GetReturnValue().Set(res);
}

} // B2JS
