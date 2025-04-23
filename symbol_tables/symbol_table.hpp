#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>
#include "./scope_table.hpp"
#include "./symbol_info.hpp"
#include "./hash_functions.hpp"

class SymbolTable{
private:
    ScopeTable * current_scope;
    int bucket_size, scope_count;
    unsigned int(*hash_function)(std::string, unsigned int);

public:
    SymbolTable(int bucket_size, unsigned int (*func)(std::string, unsigned int));
    SymbolTable(int bucket_size): SymbolTable(bucket_size, sdbm_hash) {}
    int enter_new_scope();
    int exit_scope();
    /// @brief insert a symbol inside the current scope
    /// @param s_info a pointer to the SymbolInfo object created using new operator
    /// @param pos an integer array of size 3, (scope_table_no, scope_table_row, scope_table_column) to point out the exact location of the symbol in SymbolTable
    /// @return true if insertion was successfull, false otherwise
    bool insert_symbol(SymbolInfo* s_info, int * pos=nullptr);
    /// @brief Inserts a symbol to current scope
    /// @param parts array of strings, required to create a symbol info instance. follows format [name, type, return_type(if function), function_arguments | structure elements]
    /// @param pos an integer array of size 3, (scope_table_no, scope_table_row, scope_table_column) to point out the exact location of the symbol in SymbolTable
    /// @return true if insertion was successfull, false otherwise
    bool insert_symbol(std::string* parts, int * pos=nullptr);
    /// @brief removes the desired symbol from the scope table
    /// @param symbol the desired symbols symbol name
    /// @param pos an integer array of size 3, (scope_table_no, scope_table_row, scope_table_column) to point out the exact location of the symbol in SymbolTable
    /// @return true if removal was successful, false otherwise
    bool remove_symbol(std::string symbol, int* pos=nullptr);
    /// @brief fidning a symbol from symbol table
    /// @param symbol the required symbol's name
    /// @param pos an integer array of size 3, (scope_table_no, scope_table_row, scope_table_column) to point out the exact location of the symbol in SymbolTable
    /// @return the desired symbol info instance | nullptr
    SymbolInfo * lookup(std::string symbol, int* pos=nullptr);
    std::string current_scope_string();
    std::string all_scope_string();
    ~SymbolTable();
};


SymbolTable::SymbolTable(int bucket_size, unsigned int (*func)(std::string, unsigned int)){
    this->bucket_size = bucket_size;
    this->hash_function = func;
    this->current_scope = new ScopeTable(bucket_size, 1, func);
    this->scope_count = 1;
}

int SymbolTable::enter_new_scope(){
    this->scope_count++;
    ScopeTable * new_scope = new ScopeTable(this->bucket_size, this->scope_count);
    new_scope->set_hash_function(this->hash_function);
    new_scope->set_parent_scope(this->current_scope);
    this->current_scope = new_scope;
    return current_scope->get_scope_no();
}

int SymbolTable::exit_scope(){
    if(this->current_scope == nullptr) return -1;
    if(this->current_scope->get_scope_no() == 1) return -1;
    ScopeTable* removable = this->current_scope;
    this->current_scope = this->current_scope->get_parent_scope();
    int scope_no = removable->get_scope_no();
    delete removable;
    return scope_no;
}

bool SymbolTable::insert_symbol(SymbolInfo* s_info, int* pos){
    if (pos==nullptr)
        return this->current_scope->insert_symbol(s_info, nullptr);
    pos[0] = this->current_scope->get_scope_no();
    return this->current_scope->insert_symbol(s_info, pos+1);
}

bool SymbolTable::insert_symbol(std::string* parts, int * pos){
    SymbolInfo * s_info;
    if(parts[1]=="FUNCTION")
        s_info = new FunctionSymbolInfo(parts[0], parts[1], parts[2], parts+3);
    else if(parts[1]=="STRUCT" || parts[1]=="UNION")
        s_info = new StructSymbolInfo(parts[0], parts[1], parts+2);
    else
        s_info = new SymbolInfo(parts[0], parts[1]);
    return this->insert_symbol(s_info, pos);
}

SymbolInfo* SymbolTable::lookup(std::string symbol, int* pos){
    SymbolInfo* smbl;
    int * k = nullptr;
    if (pos != nullptr) k = pos+1;
    for(ScopeTable* curr = this->current_scope; curr!=nullptr; curr=curr->get_parent_scope()){
        smbl = curr->lookup(symbol, k);
        if (smbl != nullptr){
            if (pos != nullptr) pos[0] = curr->get_scope_no();
            return smbl;
        }
    }
    return nullptr;
}

bool SymbolTable::remove_symbol(std::string symbol, int* pos){
    SymbolInfo* smbl;
    bool success = false;
    int * k = nullptr;
    if (this->current_scope == nullptr) return false;
    if (pos != nullptr){
        k = pos+1;
        pos[0] = this->current_scope->get_scope_no();
    }
    return this->current_scope->delete_symbol(symbol, k);
    // for(ScopeTable* curr = this->current_scope; curr!=nullptr; curr=curr->get_parent_scope()){
    //     success = curr->delete_symbol(symbol, k);
    //     if (success){
    //         if (pos != nullptr) pos[0] = curr->get_scope_no();
    //         return true;
    //     }
    // }
}


std::string SymbolTable::current_scope_string(){
    return this->current_scope->printable_scope_string("\t");
}

std::string SymbolTable::all_scope_string(){
    std::string represent = "";
    ScopeTable* curr = nullptr;
    int i=1;
    for(curr = this->current_scope; curr != nullptr; curr = curr->get_parent_scope(), i++){
        represent += curr->printable_scope_string(std::string(i, '\t'));        
        represent += "\n";
    }
    return represent;
}


SymbolTable::~SymbolTable(){
    ScopeTable* curr = this->current_scope, *temp;
    while(curr != nullptr){
        temp = curr;
        curr = curr->get_parent_scope();
        delete temp;
    }
}



#endif