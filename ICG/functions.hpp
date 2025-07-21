#ifndef _FUNCTIONS_HPP_
#define _FUNCTIONS_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <utility>
#include <stack>
#include <vector>
#include "../symbol_tables/symbol_table.hpp"

#define INDENT "    "


class Label{
public:
    std::string label;
    int use_count=0;
    int ref_count=0;
    Label(std::string l);
};


extern std::string definitions[];

extern std::vector<std::pair<Label*, Label*>> tf_labels;
extern std::stack<std::string> loop_label;
extern std::vector<Label *> label_stream;
extern std::vector<std::pair<int, int>> tf_label_use_count;
extern std::stack<std::pair<bool, bool>> tf_jumpable;


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
void move(std::string a, std::string b, bool preserve=false, int line=-1);
void mulop_asmcode(std::string mulop);
Label* new_label();
std::string new_loop_label();
void generate_new_tf_labels(bool true_label=true, bool false_label=true);
void place_true_label();
void place_false_label();
void pop_used_tf_label();
void place_loop_label();
void place_jump_loop();
void pop_loop_label();
void write_assign_asmcode(std::string mem, bool is_simple);
void jump_to(std::string label);
std::string get_new_single_label();
void place_single_label();
void pop_single_label();
void jump_to_current_single_label();
void set_jumpable(bool tjump=true, bool fjump=true);
void compare_asmcode(std::string cmpop, bool pop_op1, std::string op1="CX", std::string op2="AX");
void jump_to_true();
void jump_to_false();
void simple_to_conditionals(bool is_simple);
void log(std::string msg);
void finalize_func_params(std::string fid);
void insert_params_to_function(std::string vid);
void return_statement();
void push_asmcode(std::string op);
void pop_asmcode(std::string op);
void negate_operation(std::string addop, std::string operand);

#endif