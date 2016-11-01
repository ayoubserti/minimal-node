#pragma once
//stream representation

#include<stdio.h>
#include<string>


class EventLoop;
struct Handler;

class Stream {

public:
    virtual ~Stream(){};
    virtual unsigned int Get(char* outData, unsigned int len) =0;
    virtual unsigned int Put(const char* inData, unsigned int len) =0;
    virtual bool isEOF() =0;
    virtual void Close() =0;
    
};


/*
    input file stream
*/
class IFileStream  : public Stream{

    public:
		virtual ~IFileStream();
        unsigned int Get(char* outData, unsigned int len);
        unsigned int Put( const char* inData,unsigned int len);
		bool isEOF();
        void Close();
        IFileStream(EventLoop* eventLoop, FILE* inFD);

        static Stream* CreateIFileStream(EventLoop* eventLoop, const std::string& inFileName); 
    private:
        EventLoop* eventloop_;
        FILE* fd_;
        

};