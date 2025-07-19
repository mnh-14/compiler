#ifndef SYMBOL_INFO_HPP
#define SYMBOL_INFO_HPP


#include <string>
#include <iostream>
#include "./tools.hpp"
#include "./types.hpp"



class SymbolInfo {
private:
    std::string symbol;
    std::string symbol_type;
    SymbolInfo* next_symbolinfo = nullptr;
    Type* info_type = nullptr;
    MemLocation *memlocation;
public:
    SymbolInfo(std::string symbol, std::string symbol_type);
    SymbolInfo();
    void set_symbol(std::string symbol);
    void set_symbol_type(std::string stype);
    std::string get_symbol();
    std::string get_symbol_type();
    void set_next_symbolinfo(SymbolInfo* s_info);
    SymbolInfo* get_next_symbolinfo();
    void set_type(Type * t);
    Type* get_type();
    void set_memeory(MemLocation *ml);
    MemLocation* get_memory();
    virtual std::string get_string_repr(std::string lbrace, std::string rbrace);
    virtual std::string get_string_repr();
    virtual ~SymbolInfo();
};


template <typename TYPE>
class SimplePair{
private:
    TYPE first, second;
public:
    SimplePair(std::string str1, std::string str2){
        first = str1;
        second = str2;
    }
    SimplePair(){}
    TYPE getfirst(){ return first; }
    TYPE getsecond(){ return second; }
    
};

// A new Function Symbol Info class to store info on Function symbols
class FunctionSymbolInfo: public SymbolInfo{
private:
    std::string return_type;
    std::string* argument_types = nullptr;
public:
    FunctionSymbolInfo(std::string symbol, std::string symbol_type, std::string rtype, std::string* arguments);
    FunctionSymbolInfo(std::string symbol, std::string symbol_type, std::string rtype);
    void set_arguments(std::string* args);
    FunctionSymbolInfo();
    ~FunctionSymbolInfo();
    std::string get_string_repr(std::string lbrace, std::string rbrace) override;
};


// Structure Symbol Info
class StructSymbolInfo: public SymbolInfo{
private:
    SimplePair<std::string> ** strucsymbols;

public:
    StructSymbolInfo(std::string symbol, std::string symbol_type, std::string* symbols);
    StructSymbolInfo(std::string symbol, std::string symbol_type);
    StructSymbolInfo();
    void set_struc_symbols(std::string * symbols);
    ~StructSymbolInfo();
    std::string get_string_repr(std::string lbrace, std::string rbrace) override;
};




#endif