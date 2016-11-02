#pragma once 



#include "libplatform/libplatform.h"
#include "v8.h"

#include "eventloop.h"

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
 public:
  virtual void* Allocate(size_t length) {
    void* data = AllocateUninitialized(length);
    return data == NULL ? data : memset(data, 0, length);
  }
  virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
  virtual void Free(void* data, size_t) { free(data); }
};

void Print(const v8::FunctionCallbackInfo<v8::Value>& args);
void NextTick(const v8::FunctionCallbackInfo<v8::Value>& args);
void SetTimeOut(const v8::FunctionCallbackInfo<v8::Value>& args);

//read file synchronously
void ReadFile(const v8::FunctionCallbackInfo<v8::Value>& args);

//read file async
void ReadFileAsync(const v8::FunctionCallbackInfo<v8::Value>& args);

extern EventLoop* eventLoop;



