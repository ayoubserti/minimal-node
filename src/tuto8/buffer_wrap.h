#pragma once
#include "v8.h"

class Buffer;

class Buffer_Wrap
{
    

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void SetByte(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void GetLength(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void GetData(const v8::FunctionCallbackInfo<v8::Value>& args);

    static v8::Persistent<v8::FunctionTemplate> buffer_;

    Buffer*  buffer_ptr_;

	v8::Persistent<v8::Object> This_ptr_;

public:
    static void RegisterBufferClass( v8::Local<v8::Object> context);
	~Buffer_Wrap();

};