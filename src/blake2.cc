#include "blake2.hh"
#include <iostream>

#define B2JS_UPDATE_CAST(fn) \
	reinterpret_cast<int (*)(void*, const uint8_t*, uint64_t)>(fn);
#define B2JS_DIGEST_CAST(fn) \
	reinterpret_cast<int (*)(void*, uint8_t*, uint8_t)>(fn);

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
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(String::NewFromUtf8(isolate, "Blake2"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	NODE_SET_PROTOTYPE_METHOD(tpl, "update", Update);
	NODE_SET_PROTOTYPE_METHOD(tpl, "digest", Digest);

	constructor.Reset(isolate, tpl->GetFunction());
}


void Blake2::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();

	if (!args.IsConstructCall()) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Invalid instantiation!"));
		return;
	}

	string algorithm = string(*String::Utf8Value(args[0]->ToString()));
	string key;
	uint32_t outlen = 0;

	if (!args[1]->IsUndefined()) {
		if (!args[1]->IsString()) {
			isolate->ThrowException(String::NewFromUtf8(isolate, "Key must be a string!"));
		}

		key = string(*String::Utf8Value(args[1]->ToString()));
	}

	if (!args[2]->IsUndefined()) {
		if (!args[2]->IsUint32()) {
			isolate->ThrowException(String::NewFromUtf8(isolate, "Hash length must be an integer!"));
			return;
		}

		outlen = args[2]->Uint32Value();
	}

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

		instance->update = B2JS_UPDATE_CAST(blake2s_update);
		instance->digest = B2JS_DIGEST_CAST(blake2s_final);
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

		instance->update = B2JS_UPDATE_CAST(blake2sp_update);
		instance->digest = B2JS_DIGEST_CAST(blake2sp_final);
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

		instance->update = B2JS_UPDATE_CAST(blake2b_update);
		instance->digest = B2JS_DIGEST_CAST(blake2b_final);
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

		instance->update = B2JS_UPDATE_CAST(blake2bp_update);
		instance->digest = B2JS_DIGEST_CAST(blake2bp_final);
	} else {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Unknown algorithm!"));
		return;
	}

	instance->outlen = outlen;

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

	if (args[0]->IsUndefined()) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "No data was passed to the update method!"));
		return;
	}

	if (!node::Buffer::HasInstance(args[0])) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Data type must be Buffer!"));
		return;
	}

	int res = instance->update(
		&instance->state,
		reinterpret_cast<const uint8_t*>(node::Buffer::Data(args[0])),
		node::Buffer::Length(args[0]));

	if (res != 0) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Internal blake2 error!"));
		return;
	}

	args.GetReturnValue().Set(args.Holder());
}

void Blake2::Digest(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	Blake2* instance = ObjectWrap::Unwrap<Blake2>(args.Holder());

	uint8_t hash[instance->outlen];

	if (instance->digest(&instance->state, hash, instance->outlen) != 0) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Internal blake2 error!"));
		return;
	}

	//FIXME node::Buffer::New(isolate, data, length) crashes
	Local<Object> res = node::Buffer::New(isolate, sizeof(hash)).ToLocalChecked();
	memcpy(node::Buffer::Data(res), hash, sizeof(hash));

	args.GetReturnValue().Set(res);
}

} // B2JS
