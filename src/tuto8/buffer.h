
#pragma once 
class Buffer
{
    unsigned char* data_;
    unsigned long long len_;

public:
    
    enum CharSet
    {
        eASCII = 1,
        eHEX,
        eUTF8
    };

    Buffer(unsigned char* data, unsigned long long length);
	Buffer(unsigned long long length);
    Buffer(const Buffer& other);
    ~Buffer();

	unsigned long long GetLength() const;
    void SetByte(unsigned long long position,unsigned char byte);
    const unsigned char* GetData() const;
	const unsigned char  GetByte(unsigned long long index) const;

    
};