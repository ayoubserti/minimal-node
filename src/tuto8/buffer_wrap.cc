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
#include <string>
using namespace v8;


void Buffer_Wrap::RegisterBufferClass(v8::Local<v8::Object> context)
{

	Isolate* isolate = context->GetIsolate();

	v8::Local<v8::FunctionTemplate> buffer=FunctionTemplate::New(context->GetIsolate(), New);
	buffer->InstanceTemplate()->SetInternalFieldCount(1);

	buffer->PrototypeTemplate()->Set(isolate, "length", FunctionTemplate::New(isolate, GetLength));
	buffer->PrototypeTemplate()->Set(isolate, "toString", FunctionTemplate::New(isolate, GetData));
	buffer->PrototypeTemplate()->Set(isolate, "setByte", FunctionTemplate::New(isolate, SetByte));
	
	buffer_.Reset(isolate,buffer);
	
	
	/*add buffer to context*/
	context->Set(String::NewFromUtf8(isolate, "Buffer", NewStringType::kNormal).ToLocalChecked(), buffer_.Get(isolate)->GetFunction());
}

v8::Persistent<v8::FunctionTemplate> Buffer_Wrap::buffer_;

//Buffer object is a function invoked by constructor `new`
// var buffer = new Buffer(len)
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
		
		args.GetReturnValue().Set(args.This());

	}
}


void Buffer_Wrap::SetByte(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	if (args.Length() > 1)
	{
		if ((args[1]->IsString() || args[1]->IsNumber())  & args[0]->IsNumber())
		{
			Buffer_Wrap* obj = reinterpret_cast<Buffer_Wrap*>(args.This()->GetAlignedPointerFromInternalField(0));
			if (obj != nullptr)
			{
				int32_t position = args[0]->ToInteger()->Value();
				unsigned char value = 0;
				auto ptr = obj->buffer_ptr_;

				if (args[1]->IsString())
				{
					String::Utf8Value utf8value(args[1]->ToString());
					value = **utf8value;
				}
				else
				{
					value = args[1]->ToInteger()->Value() & 0xFF;
				}

				ptr->SetByte(position, value);
				args.GetReturnValue().Set(value);
			}
		}
		
	}
	
}


void Buffer_Wrap::GetLength(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Buffer_Wrap* ptr = reinterpret_cast<Buffer_Wrap*>(args.This()->GetAlignedPointerFromInternalField(0));
	args.GetReturnValue().Set((int32_t)ptr->buffer_ptr_->GetLength());
}

void Buffer_Wrap::GetData(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	Isolate* isolate = args.GetIsolate();
	if (args.Length() > 0)
	{
		if (args[0]->IsString())
		{
			Buffer_Wrap* obj = reinterpret_cast<Buffer_Wrap*>(args.This()->GetAlignedPointerFromInternalField(0));
			if (obj != nullptr)
			{
				int32_t charset = args[0]->ToInt32()->Value();
				auto ptr = obj->buffer_ptr_;
				const unsigned char* data = ptr->GetData();
				String::Utf8Value value(args[0]->ToString());
				std::string strVal(*value);

				if (strVal=="hex")
				{
					
				}
				else if (strVal == "ascii")
				{
					//TODO
				}
				else if (strVal == "utf8")
				{
					Local<String> ret = String::NewFromUtf8(isolate, (const char*)data, NewStringType::kNormal, ptr->GetLength()).ToLocalChecked();
					args.GetReturnValue().Set(ret);
				}
				
			}
		}

	}

}
