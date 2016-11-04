/*
    @author Ayoub Serti
    @email  ayb.serti@gmail.com
    @file   buffer.cc
    @abstract
            small implementation of buffer class
*/

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "buffer.h"


Buffer::Buffer(unsigned char* data, unsigned long long length)
{
    //this constructor copy data
    data_ = (unsigned char*)malloc(length);
	memcpy(data_, data, length);
	len_ = length;
    
}

Buffer::~Buffer()
{
    free(data_);
}

Buffer::Buffer(const Buffer& other)
{
    len_ = other.len_;
    data_ = (unsigned char*)malloc(len_);
	memcpy(data_,other.data_,len_);
}

unsigned long long Buffer::GetLength() const
{
    return len_;
}

void Buffer::SetByte(unsigned long long position,unsigned char byte)
{
    if(position<len_)
        data_[position] = byte;
}


const unsigned char* Buffer::GetData() const
{
    return data_;
}

Buffer::Buffer(unsigned long long length)
{
	len_ = length;
	data_ = (unsigned char*)calloc(len_,1);
	
}