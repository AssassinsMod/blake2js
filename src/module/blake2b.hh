#ifndef BLAKE2B_H
#define BLAKE2B_H

#include <node.h>
#include <node_buffer.h>
#include <node_object_wrap.h>

#include "lib/BLAKE2/sse/blake2.h"

using namespace v8;

class Blake2b : public node::ObjectWrap {
	public:
		/*! Initialize the class.
		 * Performs a serie of operations needed for the class to be
		 * usable. Only needs to be called once on module load.
		 * @param exports   Module api.
		 */
		static void Init(Local<Object> exports);

		/*! Wrappable constructor.
		 * This is the constructor that is called from javascript
		 * @param args   function call parameters.
		 */
		static void New(const FunctionCallbackInfo<Value>& args);

		/*! Object factory.
		 * This is used to create new instances of the class.
		 * @param args   function call parameters.
		 */
		static void NewInstance(const FunctionCallbackInfo<Value>& args);

	private:
		explicit Blake2b();
		~Blake2b();

		static Persistent<Function> constructor; /*!< V8 constructor template. */
		static void Update(const FunctionCallbackInfo<Value>& args);
		static void Digest(const FunctionCallbackInfo<Value>& args);

		blake2b_state state; /*!< Blake2b internal state. */
};

#endif
