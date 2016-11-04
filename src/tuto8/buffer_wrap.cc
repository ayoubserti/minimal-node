/*
    @author Ayoub Serti
    @email  ayb.serti@gmail.com
    @file   buffer_wrap.cc
    @abstract
            small implementation of Buffer JS ojbect
*/


#include "v8.h"
#include "buffer_wrap.h"
#include "buffer.h"

using namespace v8;
//Buffer object is a function invoked by constructor `new`
// var buffer = new Buffer(len)

void Buffer_Wrap::RegisterBufferClass(v8::Local<v8::Object> context)
{

	Isolate* isolate = context->GetIsolate();

	v8::Local<v8::FunctionTemplate> buffer=FunctionTemplate::New(context->GetIsolate(), New);
	buffer->PrototypeTemplate()->SetInternalFieldCount(1);

	buffer->PrototypeTemplate()->Set(isolate, "getLength", FunctionTemplate::New(isolate, GetLength));
	buffer->PrototypeTemplate()->Set(isolate, "getData", FunctionTemplate::New(isolate, GetData));
	buffer->PrototypeTemplate()->Set(isolate, "setByte", FunctionTemplate::New(isolate, SetByte));
	
	buffer_.Reset(isolate,buffer);
	
	
	/*add buffer to context*/
	context->Set(String::NewFromUtf8(isolate, "buffer", NewStringType::kNormal).ToLocalChecked(), buffer_.Get(isolate)->GetFunction());
}

v8::Persistent<v8::FunctionTemplate> Buffer_Wrap::buffer_;


void Buffer_Wrap::New(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Isolate* isolate = args.GetIsolate();
	if (args.IsConstructCall())
	{
		
		//if new constructor
		Buffer_Wrap* ptr = new Buffer_Wrap();
		//buf_wrap->This_ptr_.Reset(args.This());
		ptr->This_ptr_.Reset(isolate, args.This());
		
		ptr->This_ptr_.Get(isolate)->SetAlignedPointerInInternalField(0, ptr);
		
		if (args.Length() > 0 && args[0]->IsNumber())
		{
			int32_t len = args[0]->ToNumber()->Int32Value();
			ptr->buffer_ptr_ = new Buffer(len);
		}
		

	}
}


void Buffer_Wrap::SetByte(const v8::FunctionCallbackInfo<v8::Value>& args)
{

}


void Buffer_Wrap::GetLength(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Buffer_Wrap* ptr = reinterpret_cast<Buffer_Wrap*>(args.This()->GetAlignedPointerFromInternalField(0));
}

void Buffer_Wrap::GetData(const v8::FunctionCallbackInfo<v8::Value>& args)
{

}
