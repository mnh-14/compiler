#ifndef _TYPES_H_
#define _TYPES_H_
#include <string>


class Type{
public:
    std::string type_string;
    int size;
    Type(std::string t, int s);
    Type();
    static Type * construct_type(std::string t);
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
    virtual std::string get_location();
    MemLocation();
};

class GlobalMemLocation: public MemLocation{
public:
    GlobalMemLocation(std::string name);
    GlobalMemLocation();
    std::string get_location() override;
};


#endif