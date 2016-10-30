#pragma once 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <queue>

//used for this_thread::yield()
#include <thread>

#include <chrono>

#include "libplatform/libplatform.h"
#include "v8.h"

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


struct Handler
{
	std::chrono::time_point<std::chrono::system_clock> start, end;
	v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function> > callback;
};


extern std::queue<Handler> eventQueue;