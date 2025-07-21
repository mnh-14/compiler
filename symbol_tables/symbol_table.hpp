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
    int stack_offset = -1;
    unsigned int(*hash_function)(std::string, unsigned int);

public:
    SymbolTable(int bucket_size, unsigned int (*func)(std::string, unsigned int));
    SymbolTable(int bucket_size);
    int enter_new_scope();
    int exit_scope();
    int get_space_for_local_variable(Type * t);
    int get_current_stack_offset();
    void reset_stack_offset();
    /// @brief insert a symbol inside the current scope
    /// @param s_info a pointer to the SymbolInfo object created using new operator
    /// @param pos an integer array of size 3, (scope_table_no, scope_table_row, scope_table_column) to point out the exact location of the symbol in SymbolTable
    /// @return true if insertion was successfull, false otherwise
    bool insert_symbol(SymbolInfo* s_info, int * pos=nullptr);
    /// @brief Inserts a symbol to current scope
    /// @param parts array of strings, required to create a symbol info instance. follows format [name, type, return_type(if function), function_arguments | structure elements]
    /// @param pos an integer array of size 3, (scope_table_no, scope_table_row, scope_table_column) to point out the exact location of the symbol in SymbolTable
    /// @return true if insertion was successfull, false otherwise
    SymbolInfo* insert_symbol(std::string* parts, int * pos=nullptr);
    /// @brief removes the desired symbol from the scope table
    /// @param symbol the desired symbols symbol name
    /// @param pos an integer array of size 3, (scope_table_no, scope_table_row, scope_table_column) to point out the exact location of the symbol in SymbolTable
    /// @return true if removal was successful, false otherwise
    bool remove_symbol(std::string symbol, int* pos=nullptr);
    /// @brief fidning a symbol from symbol table
    /// @param symbol the required symbol's name
    /// @param pos an integer array of size 3, (scope_table_no, scope_table_row, scope_table_column) to point out the exact location of the symbol in SymbolTable
    /// @return the desired symbol info instance | nullptr
    SymbolInfo * lookup(std::string symbol, int* pos=nullptr, std::string * scope_label=nullptr);
    std::string current_scope_string();
    std::string all_scope_string();
    bool is_global_scope();
    ~SymbolTable();
};


#endif