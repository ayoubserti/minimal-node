#include "stream.h"

#include "eventloop.h"

IFileStream::IFileStream(EventLoop* eventLoop,FILE* inFD)
:eventloop_(eventLoop),
fd_(inFD)
{
    //when a stream file is create
    //notify eventloop that a new prepare handler is ready to be add to loop
    Handler* prepare= new Handler();
	prepare->kind = ePrepare;
	
    
}

IFileStream::~IFileStream()
{
    Close();
}


unsigned int IFileStream::Get(char* outData, unsigned int len)
{
    return  fread(outData,1,len,fd_);
}

unsigned int IFileStream::Put( const char* outData, unsigned int len)
{
    return  fwrite(outData,1,len,fd_);
}

bool IFileStream::isEOF()
{
    return feof(fd_);    
}

void  IFileStream::Close()
{
    fclose(fd_);
}
Stream* CreateIFileStream(EventLoop* eventLoop, const std::string& inFileName)
{

    FILE* fd = fopen(inFileName.c_str(),"rb"); //open for read binary
    if( fd == nullptr) return nullptr; 
    Stream* stream = new IFileStream(eventLoop,fd);

    return stream;
}

