#ifndef _TYPES_H_
#define _TYPES_H_
#include <string>


class Type{
public:
    std::string type_string;
    int size;
    Type(std::string t, int s);
    virtual int get_size();
    virtual ~Type();
};

class IntType: public Type{
public:
    IntType();
};

class CharType: public Type{
public:
    CharType();
};


#endif