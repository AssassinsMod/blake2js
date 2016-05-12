#include "utils.hh"

using std::string;
using v8::Exception;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void B2JS::Utils::ParseOptions(Isolate* isolate, Local<Value> arg, blake2b_constant max_keylen, string& key, uint8_t& outlen) {
	if (!arg->IsObject()) {
		isolate->ThrowException(Exception::SyntaxError(
			String::NewFromUtf8(isolate, "Options must be passed as an object!")));
		return;
	}

	Local<Object> options = arg->ToObject();

	Local<String> _key = String::NewFromUtf8(isolate, "key"),
	           _length = String::NewFromUtf8(isolate, "length");

	// options.key
	if (options->Has(_key)) {
		Local<Value> _key_val = options->Get(_key);

		if (node::Buffer::HasInstance(_key_val)) {
			Local<Object> buffer = _key_val->ToObject();

			char* key_data = node::Buffer::Data(buffer);
			size_t key_length = node::Buffer::Length(buffer);

			if (key_length <= max_keylen) {
				key = string(key_data, key_length);
			} else {
				isolate->ThrowException(Exception::SyntaxError(
					String::NewFromUtf8(isolate, "Exceeded max key length!")));
				return;
			}
		} else if (_key_val->IsString()) {
			//TODO Add stringed keys support
			isolate->ThrowException(Exception::ReferenceError(
				String::NewFromUtf8(isolate, "String key not supported yet!")));
			return;
		} else {
			isolate->ThrowException(Exception::SyntaxError(
				String::NewFromUtf8(isolate, "Unsupported key format!")));
			return;
		}
	}

	// options.length
	if (options->Has(_length)) {
		Local<Value> _length_val = options->Get(_length);

		if (_length_val->IsUint32()) {
			uint32_t value = _length_val->Uint32Value();

			if (value < outlen) {
				outlen = value;
			} else {
				isolate->ThrowException(Exception::RangeError(
					String::NewFromUtf8(isolate, "Exceeded maximum outlen")));
				return;
			}
		} else {
			isolate->ThrowException(Exception::SyntaxError(
				String::NewFromUtf8(isolate, "Length must be an integer")));
			return;
		}
	}
}
