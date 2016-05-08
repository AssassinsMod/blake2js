#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <nan.h>

#include "objects/blake2b.hh"

void CreateHash(const Nan::FunctionCallbackInfo<v8::Value> &info) {
	// default values
	std::string algorithm = "blake2b";
	uint8_t out_length = BLAKE2B_OUTBYTES;

	// parse args
	if (info.Length() > 0) { // algorithm
		if (!info[0]->IsString()) {
			Nan::ThrowError(Nan::TypeError("Algorithm must be a string"));
		}

		std::string arg0 = std::string(*(v8::String::Utf8Value(info[0]->ToString())));

		if (arg0 == "blake2b" || arg0 == "blake2bp") {
			algorithm = arg0;
		} else if (arg0 == "blake2s" || arg0 == "blake2sp") {
			algorithm = arg0;
			out_length = BLAKE2S_OUTBYTES;
		} else {
			Nan::ThrowError(Nan::SyntaxError("Undefined algorithm"));
		}

		if (info.Length() > 1) { // out_length
			if (!info[1]->IsUint32()) {
				Nan::ThrowError(Nan::TypeError("Length must be a positive integer"));
			}

			uint32_t arg1 = info[1]->ToUint32()->Value();

			if (arg1 <= out_length && arg1 != 0) {
				out_length = arg1;
			} else {
				Nan::ThrowError(Nan::TypeError("Length out of boundaries"));
			}
		}
	}



	// return instance
	info.GetReturnValue().Set(obj);
}

/*
void Blake2bInit(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	blake2b_state state;
	uint8_t length;

	if (info[0]->IsNumber()) {
		uint32_t arg0 = info[0]->Uint32Value();

		length = (arg0 < BLAKE2B_OUTBYTES && arg0 != 0) ? arg0 : BLAKE2B_OUTBYTES;
	}

	blake2b_init(&state, length);

	// return state buffer
	info.GetReturnValue().Set(Nan::CopyBuffer((char*) &state, sizeof(state)).ToLocalChecked());
}
*/

void Init(v8::Local<v8::Object> exports) {
	//exports->Set(Nan::New("Blake2bInit").ToLocalChecked(),
	//             Nan::New<v8::FunctionTemplate>(Blake2bInit)->GetFunction());
	Blake2b::Init();
}

NODE_MODULE(hello, Init)
