#include "utils.hh"

namespace B2JS {

using v8::Isolate;
using v8::Local;
using v8::Value;
using v8::String;
using std::string;
using v8::Object;

void Utils::PrintHex(const uint8_t* data, size_t length) {
	printf("| ================== DATA =================== |\n");

	for (size_t i = 0; i < length; i++) {
		printf("%02x ", data[i]);

		if ((i + 1) % 16 == 0) {
			printf("\n");
		}
	}
}

void Utils::ParseOptions(Isolate* isolate, Local<Value> arg, uint8_t max_keylen, string& key, uint8_t& outlen) {
	if (!arg->IsObject()) {
		isolate->ThrowException(String::NewFromUtf8(isolate, "Invalid options format!"));
		return;
	}

	Local<Object> options = arg->ToObject();

	Local<String> _key = String::NewFromUtf8(isolate, "key"),
	           _length = String::NewFromUtf8(isolate, "length");

	// key
	if (options->Has(_key)) {
		Local<Value> _key_val = options->Get(_key);

		if (node::Buffer::HasInstance(_key_val)) {
			Local<Object> buffer = _key_val->ToObject();

			char* key_data = node::Buffer::Data(buffer);
			size_t key_length = node::Buffer::Length(buffer);

			if (key_length <= max_keylen) {
				key = string(key_data, key_length);
			} else {
				isolate->ThrowException(String::NewFromUtf8(isolate, "Exceeded max key length!"));
				return;
			}
		} else if (_key_val->IsString()) {
			//TODO Add stringed keys support
			isolate->ThrowException(String::NewFromUtf8(isolate, "String key not supported yet!"));
			return;
		} else {
			isolate->ThrowException(String::NewFromUtf8(isolate, "Unsupported key format!"));
			return;
		}
	}

	// length
	if (options->Has(_length)) {
		Local<Value> _length_val = options->Get(_length);

		if (_length_val->IsUint32()) {
			uint32_t value = _length_val->Uint32Value();

			if (value < outlen) {
				outlen = value;
			} else {
				isolate->ThrowException(String::NewFromUtf8(isolate, "Exceeded maximum outlen"));
				return;
			}
		} else {
			isolate->ThrowException(String::NewFromUtf8(isolate, "Length must be an integer"));
			return;
		}
	}
}

}
