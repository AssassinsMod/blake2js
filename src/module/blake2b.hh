#ifndef BLAKE2B_H
#define BLAKE2B_H

#include <cstdio>
#include <string>
#include <node.h>
#include <node_buffer.h>
#include <node_object_wrap.h>
#include "../../lib/BLAKE2/sse/blake2.h"

class Blake2b : public node::ObjectWrap {
	public:
		/*! Initialize class.
		 * Performs preliminary operations to make the class wrappable
		 * by the v8 engine.
		 * @param isolate  Isolate.
		 */
		static void Init(v8::Isolate* isolate);


		/*! Create a new instance of the class.
		 *
		 * @param info  Call info.
		 */
		static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);

	private:
		// Class constructor/destructor.
		explicit Blake2b();
		~Blake2b();

		void init(int length, std::string key);

		uint8_t outlen; /*!< Output hash length. */
		blake2b_state* state; /*!< Blake2b internal state. */
		static v8::Persistent<v8::Function> constructor; /*!< V8 constructor template. */

		/*! Wrappable constructor.
		 * This is the method called when the constructor is invoked in javascript.
		 * @param info  Call info.
		 */
		static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

		/*! Update the current hash with given data.
		 *
		 * @param args  Data to update the hash with.
		 */
		static void Update(const v8::FunctionCallbackInfo<v8::Value>& args);

		/*! Finalize the current hash and return the result.
		 *
		 * @param args  Ignored.
		 */
		static void Digest(const v8::FunctionCallbackInfo<v8::Value>& args);
};

#endif
