#ifndef SCOPE_TABLE_HPP
#define SCOPE_TABLE_HPP


#include <iostream>
#include <string>
#include "symbol_info.hpp"
#include "./tools.hpp"


class ScopeTable{
private:
    int bucket_size, scope_no;
    std::string label="1";
    int sub_scope_count=0;
    unsigned int(*hash_function)(std::string, unsigned int);
    SymbolInfo ** symbol_infos = nullptr;
    ScopeTable * parent_scope = nullptr;

public:
    ScopeTable(int bucket_size, int scope_no, unsigned int (*func)(std::string, unsigned int));
    ScopeTable(int bucket_size, int scope_no);
    void set_hash_function(unsigned int (*func)(std::string, unsigned int));
    void set_parent_scope(ScopeTable* parent);
    ScopeTable* get_parent_scope();
    int get_scope_no();
    ~ScopeTable();
    /// @brief Inserts the symbol into the scope table
    /// @param s_info the SymbolInfo object pointer the be inserted
    /// @param insertion_pos an int array of 2 size: [hash table index, chained list index] indicating the position of the symbolInfo
    /// @return True or False depending on whether the insersion was successfull or not
    bool insert_symbol(SymbolInfo* s_info, int * insertion_pos=nullptr);
    /// @brief Find the symbol Info with desired symbol name
    /// @param symbol the symbol name used to search
    /// @param pos an int array of 2 size: [hash table index, chained list index] indicating the position of the symbolInfo
    /// @return a pointer to the desired SymbolInfo instance or nullptr
    SymbolInfo* lookup(std::string symbol, int* pos=nullptr);
    /// @brief delete a certain symbol
    /// @param symbol the desired symbol name to be deleted
    /// @param pos an int array of 2 size: [hash table index, chained list index] indicating the position of the symbolInfo
    /// @return True if deletion was successful, False otherwise
    bool delete_symbol(std::string symbol, int* pos=nullptr);
    std::string get_label();
    std::string printable_scope_string(std::string indent);
    std::string printable_scope_string();
};


#endif