#include "blake2.hh"
#include <iostream>

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

Blake2::Blake2() {}
Blake2::~Blake2() {}

Persistent<Function> Blake2::constructor;


void Blake2::Init(Isolate *isolate) {
	// Prepare template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(String::NewFromUtf8(isolate, "Blake2"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Set prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "update", Update);
	NODE_SET_PROTOTYPE_METHOD(tpl, "digest", Digest);

	// Prepare constructor
	constructor.Reset(isolate, tpl->GetFunction());
}


void Blake2::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();

	if (!args.IsConstructCall()) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Invalid instantiation!"));
		return;
	}

	// Create instance
	string algorithm = string(*String::Utf8Value(args[0]->ToString()));
	string key;
	uint32_t outlen = 0;

	// Parse arguments
	if (!args[1]->IsUndefined()) {
		key = string(*String::Utf8Value(args[1]->ToString()));
	}

	if (!args[2]->IsUndefined()) {
		if (!args[2]->IsUint32()) {
			isolate->ThrowException(String::NewFromUtf8(isolate, "Invalid output length!"));
			return;
		}

		outlen = args[2]->Uint32Value();
	}

	// Instanciate
	Blake2* instance = new Blake2();

	//TODO find a better way to do this
	if (algorithm == "blake2s") {
		if (outlen == 0) {
			outlen = BLAKE2S_OUTBYTES;
		} else if (outlen > BLAKE2S_OUTBYTES) {
			isolate->ThrowException(String::NewFromUtf8(isolate, "Exceeded maximum output length!"));
			return;
		}

		if (key.empty()) {
			blake2s_init((blake2s_state*)&instance->state, outlen);
		} else {
			if (key.length() > BLAKE2S_KEYBYTES) {
				isolate->ThrowException(String::NewFromUtf8(isolate, "Exceeded maximum key length!"));
				return;
			}

			blake2s_init_key((blake2s_state*)&instance->state, outlen, key.c_str(), key.length());
		}

		instance->update = reinterpret_cast<int (*)(void*, const uint8_t*, uint64_t)>(blake2s_update);
		instance->digest = reinterpret_cast<int (*)(void*, uint8_t*, uint8_t)>(blake2s_final);
	} else if (algorithm == "blake2sp") {
		if (outlen == 0) {
			outlen = BLAKE2S_OUTBYTES;
		} else if (outlen > BLAKE2S_OUTBYTES) {
			isolate->ThrowException(String::NewFromUtf8(isolate, "Exceeded maximum output length!"));
			return;
		}

		if (key.empty()) {
			blake2sp_init((blake2sp_state*)&instance->state, outlen);
		} else {
			if (key.length() > BLAKE2S_KEYBYTES) {
				isolate->ThrowException(String::NewFromUtf8(isolate, "Exceeded maximum key length!"));
				return;
			}

			blake2sp_init_key((blake2sp_state*)&instance->state, outlen, key.c_str(), key.length());
		}

		instance->update = reinterpret_cast<int (*)(void*, const uint8_t*, uint64_t)>(blake2sp_update);
		instance->digest = reinterpret_cast<int (*)(void*, uint8_t*, uint8_t)>(blake2sp_final);
	} else if (algorithm == "blake2b") {
		if (outlen == 0) {
			outlen = BLAKE2B_OUTBYTES;
		} else if (outlen > BLAKE2B_OUTBYTES) {
			isolate->ThrowException(String::NewFromUtf8(isolate, "Exceeded maximum output length!"));
			return;
		}

		if (key.empty()) {
			blake2b_init((blake2b_state*)&instance->state, outlen);
		} else {
			if (key.length() > BLAKE2B_KEYBYTES) {
				isolate->ThrowException(String::NewFromUtf8(isolate, "Exceeded maximum key length!"));
				return;
			}

			blake2b_init_key((blake2b_state*)&instance->state, outlen, key.c_str(), key.length());
		}

		instance->update = reinterpret_cast<int (*)(void*, const uint8_t*, uint64_t)>(blake2b_update);
		instance->digest = reinterpret_cast<int (*)(void*, uint8_t*, uint8_t)>(blake2b_final);
	} else if (algorithm == "blake2bp") {
		if (outlen == 0) {
			outlen = BLAKE2B_OUTBYTES;
		} else if (outlen > BLAKE2B_OUTBYTES) {
			isolate->ThrowException(String::NewFromUtf8(isolate, "Exceeded maximum output length!"));
			return;
		}

		if (key.empty()) {
			blake2bp_init((blake2bp_state*)&instance->state, outlen);
		} else {
			if (key.length() > BLAKE2B_KEYBYTES) {
				isolate->ThrowException(String::NewFromUtf8(isolate, "Exceeded maximum key length!"));
				return;
			}

			blake2bp_init_key((blake2bp_state*)&instance->state, outlen, key.c_str(), key.length());
		}

		instance->update = reinterpret_cast<int (*)(void*, const uint8_t*, uint64_t)>(blake2bp_update);
		instance->digest = reinterpret_cast<int (*)(void*, uint8_t*, uint8_t)>(blake2bp_final);
	} else {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Unknown algorithm!"));
		return;
	}

	instance->outlen = outlen;


	// Wrap and return instance
	instance->Wrap(args.This());
	args.GetReturnValue().Set(args.This());
}

void Blake2::NewInstance(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	// Load constructor
	Local<Function> constr = Local<Function>::New(isolate, constructor);

	// Prepare and return instance
	const unsigned argc = 3;
	Local<Value> argv[argc] = { args[0], args[1], args[2] };
	args.GetReturnValue().Set(constr->NewInstance(argc, argv));
}

// ================ ]  PROTOTYPE  [ ================ //
void Blake2::Update(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	Blake2* instance = ObjectWrap::Unwrap<Blake2>(args.Holder());


	// Check if data has been passed
	if (args[0]->IsUndefined()) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "No data was passed!"));
		return;
	}

	if (node::Buffer::HasInstance(args[0])) { // if data is a buffer
		// Get data from buffer
		uint8_t*   data = reinterpret_cast<uint8_t*>(node::Buffer::Data(args[0]));
		uint64_t length = reinterpret_cast<uint64_t>(node::Buffer::Length(args[0]));

		// Perform update
		if (instance->update(&instance->state, data, length) == 0) {
			args.GetReturnValue().Set(args.Holder());
		} else {
			isolate->ThrowException(String::NewFromUtf8(isolate, "Internal blake2 error!"));
		}
	} else if (args[0]->IsString()) { // if data is a string
		isolate->ThrowException(String::NewFromUtf8(isolate, "String support yet to be implemented!"));
	} else {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Unsupported data format!"));
	}
}

void Blake2::Digest(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	Blake2* instance = ObjectWrap::Unwrap<Blake2>(args.Holder());

	// Create output buffer of the right length
	uint8_t hash[instance->outlen];

	// Perform digest
	if (instance->digest(&instance->state, hash, instance->outlen) == 0) {
		// Create buffer and copy data into it
		Local<Object> res = node::Buffer::New(isolate, sizeof(hash)).ToLocalChecked();
		memcpy(node::Buffer::Data(res), (char*)hash, sizeof(hash));
		/* FIXME node::Buffer::New with data crashes.
		 * For some reason node::Buffer::New(isolate, (char*)hash, sizeof(hash));
		 * doesn't work, so for now we'll manually copy the data inside the newly
		 * created buffer. */

		args.GetReturnValue().Set(res);
	} else {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Internal blake2 error!"));
	}
}

} // B2JS
