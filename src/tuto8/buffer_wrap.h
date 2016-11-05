#pragma once
#include "v8.h"

class Buffer;

class Buffer_Wrap
{
    

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void SetByte(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void GetLength(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void GetData(const v8::FunctionCallbackInfo<v8::Value>& args);

	static void IndexedGetter(uint32_t index, const v8::PropertyCallbackInfo<v8::Value>& info);

	static void IndexedSetter(	uint32_t index,	v8::Local<v8::Value> value,	const v8::PropertyCallbackInfo<v8::Value>& info);

    static v8::Persistent<v8::FunctionTemplate> buffer_;

    Buffer*  buffer_ptr_;

	v8::Persistent<v8::Object> This_ptr_;

public:
    static void RegisterBufferClass( v8::Local<v8::Object> context);
	~Buffer_Wrap();

};