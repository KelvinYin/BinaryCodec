#ifndef SIMPLE_WRITESTREAM_H 
#define SIMPLE_WRITESTREAM_H 
#include "Codec.h"

#include <string>
#include <stdint.h>

class BinaryWriteStream
{
public:
	BinaryWriteStream(std::string *data);
    const char* GetData() const { return data_->data(); }
    size_t GetSize() const { return data_->length(); }
    size_t GetCurrentPos() const{return data_->length();}

    bool Write(const char* str,size_t length);
    bool Write(double value,bool isNULL = false);
    bool Write(long value,bool isNULL = false);
    bool Write(int i,bool isNULL = false);
    bool Write(short i,bool isNULL = false);
    bool Write(char c,bool isNULL = false);
    void Flush();
    void Clear();

private:
    std::string *data_;
};

#endif // SIMPLE_WRITESTREAM_H 
