#ifndef _TYPES_H_
#define _TYPES_H_
#include <string>


class Type{
public:
    std::string type_string;
    int size;
    Type(std::string t, int s);
    Type();
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


class MemLocation{
public:
    std::string base_ptr;
    int offset;
    MemLocation(std::string bp, int off);
    std::string get_location();
    MemLocation();
};


#endif