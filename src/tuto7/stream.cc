#include "stream.h"

#include "eventloop.h"

IFileStream::IFileStream(EventLoop* eventLoop, Handler* prepare, FILE* inFD)
:eventloop_(eventLoop),
prepare_(prepare),
fd_(inFD)
{
    
    
}

IFileStream::~IFileStream()
{
    Close();
	
}


unsigned int IFileStream::Get(char* outData, unsigned int len)
{
	len = fread(outData, 1, len, fd_);
	if (len > 0)
	{
		Handler check;
		check.kind = eCheck;
		check.data = outData;
		check.len = len;
		check.parent = prepare_; //bind this check to prepare handler
		prepare_->ref++;
		eventloop_->push(check);
	}
	return len;
}

unsigned int IFileStream::Put( const char* outData, unsigned int len)
{
    return  fwrite(outData,1,len,fd_);
}

bool IFileStream::isEOF()
{
	return (feof(fd_) !=0) ? true : false;
}

void  IFileStream::Close()
{
    fclose(fd_);
}
Stream* IFileStream::CreateIFileStream(EventLoop* eventLoop,Handler* prepare, const std::string& inFileName)
{

    FILE* fd = fopen(inFileName.c_str(),"rb"); //open for read binary
    if( fd == nullptr) return nullptr; 
    Stream* stream = new IFileStream(eventLoop,prepare,fd);

    return stream;
}

