#include "stream.h"

IFileStream::IFileStream(EventLoop* eventLoop,FILE* inFD)
:eventloop_(eventloop),
:fd_(inFD)
{

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

