#ifndef _FUNCTIONS_HPP_
#define _FUNCTIONS_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <utility>
#include <stack>
#include "../symbol_tables/symbol_table.hpp"

#define INDENT "    "

extern std::string definitions[];

extern std::stack<pair<std::string, std::string>> tf_labels;


extern std::ofstream parserLogFile;
extern std::ofstream errorFile;
extern std::ofstream codeblock;
extern std::ofstream asmfile;
extern int syntaxErrorCount;
extern SymbolTable symbol_table;
extern bool is_new_scoped;
extern std::vector<std::pair<std::string, std::string>> param_list;
extern std::vector<std::string> declared_ids;
extern std::string global_type;




void insert_variable_to_symbletable_and_gencode(std::string vid);
void forced_enter_scope();
void enter_scope();
void exit_scope();
void entering_function(std::string fid, std::string rettype, int line_no);
void finishing_function(std::string fid);
void addop_asmcode(std::string addop);
std::string get_memloc(std::string id);
void move(std::string a, std::string b, int line=-1);
void mulop_asmcode(std::string mulop);
std::string new_label();

#endif