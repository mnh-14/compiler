#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>
#include "./scope_table.hpp"
#include "./symbol_info.hpp"
#include "./hash_functions.hpp"

class SymbolTable{
private:
    ScopeTable * current_scope;
    int bucket_size;
    unsigned int(*hash_function)(std::string, unsigned int);

public:
    SymbolTable(int bucket_size, unsigned int (*func)(std::string, unsigned int));
    SymbolTable(int bucket_size): SymbolTable(bucket_size, sdbm_hash) {}
    int enter_new_scope();
    int exit_scope();
    bool insert_symbol(SymbolInfo* s_info, int * pos=nullptr);
    bool insert_symbol(std::string* parts, int * pos=nullptr);
    bool remove_symbol(std::string symbol);
    SymbolInfo * lookup(std::string symbol, int* pos=nullptr);
    std::string current_scope_string();
    std::string all_scope_string();
    ~SymbolTable();
};


SymbolTable::SymbolTable(int bucket_size, unsigned int (*func)(std::string, unsigned int)){
    this->bucket_size = bucket_size;
    this->hash_function = func;
    this->current_scope = new ScopeTable(bucket_size, 1, func);
}

int SymbolTable::enter_new_scope(){
    ScopeTable * new_scope = new ScopeTable(this->bucket_size, this->current_scope->get_scope_no()+1);
    new_scope->set_hash_function(this->hash_function);
    new_scope->set_parent_scope(this->current_scope);
    this->current_scope = new_scope;
    return current_scope->get_scope_no();
}

int SymbolTable::exit_scope(){
    ScopeTable* removable = this->current_scope;
    this->current_scope = this->current_scope->get_parent_scope();
    int scope_no = removable->get_scope_no();
    delete removable;
    return scope_no;
}

bool SymbolTable::insert_symbol(SymbolInfo* s_info, int* pos=nullptr){
    return this->current_scope->insert_symbol(s_info, pos);
}

bool SymbolTable::insert_symbol(std::string* parts, int * pos=nullptr){
    SymbolInfo * s_info;
    if(parts[1]=="FUNCTION")
        s_info = new FunctionSymbolInfo(parts[0], parts[1], parts[2], parts+3);
    else if(parts[1]=="STRUCT")
        s_info = new StructSymbolInfo(parts[0], parts[1], parts+2);
    else
        s_info = new SymbolInfo(parts[0], parts[1]);
    return this->current_scope->insert_symbol(s_info, pos);
}





#endif