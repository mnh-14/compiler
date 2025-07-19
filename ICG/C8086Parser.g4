parser grammar C8086Parser;

options {
    tokenVocab = C8086Lexer;
}

@parser::header {
    #include <iostream>
    #include <fstream>
    #include <string>
    #include <cstdlib>
	#include <vector>
	#include <utility>
    #include "C8086Lexer.h"
	#include "../symbol_tables/symbol_table.hpp"
    
    #define INDENT "    "
	
	
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
}

@parser::members {
    void writeIntoparserLogFile(const std::string message) {
        if (!parserLogFile) {
            std::cout << "Error opening parserLogFile.txt" << std::endl;
            return;
        }

        parserLogFile << message << std::endl;
        parserLogFile.flush();
    }

    void writeIntoErrorFile(const std::string message) {
        if (!errorFile) {
            std::cout << "Error opening errorFile.txt" << std::endl;
            return;
        }
        errorFile << message << std::endl;
        errorFile.flush();
    }

	void write_rule(int line_no, std::string rule, std::string matched){
		writeIntoparserLogFile("Line "+std::to_string(line_no)+": " + rule + "\n\n" + matched + "\n");
	}
	void enter_scope(){
		if(is_new_scoped)
			is_new_scoped=false;
		else
			symbol_table.enter_new_scope();
	}
	void exit_scope(){
		writeIntoparserLogFile("\n"+symbol_table.all_scope_string()+"\n");
		symbol_table.exit_scope();
	}
	void fn_scope(){
		symbol_table.enter_new_scope();
		is_new_scoped=true;
	}
	
	void declare_variables(bool dl, int line, std::string type=""){
		std::cout << "Declaring variables" << std::endl;
		std::string var[2];
		bool success;
		if (dl) {
			for(auto s:declared_ids){
				std::cout << "ID: "<< s << " Declared" << std::endl;
				var[0] = s; var[1] = "ID";
				success = symbol_table.insert_symbol(var);
				if(!success){
					writeIntoErrorFile("Error at Line "+std::to_string(line)+" Multiple declaration of " + s + "\n");
				}
			}
			declared_ids.clear();
		} else {
			for(auto s:param_list){
				std::cout << "pl:ID: "<< s.first << " " << s.second << " Declared" << std::endl;
				var[0] = s.second; var[1] = "ID";
				success = symbol_table.insert_symbol(var);
				if(!success){
					writeIntoErrorFile("Error at Line "+std::to_string(line)+" Multiple declaration of " + s.second + "\n");
				}
			}
			param_list.clear();
		}
	}
	void declare_function2(std::string name, std::string ret, int line, bool warn=true){
		std::string var[2];
		bool success;
		var[0] = name; var[1] = "ID";
		success = symbol_table.insert_symbol(var);
		if(!success && warn){
			writeIntoErrorFile("Error at Line "+std::to_string(line)+" Multiple declaration of " + name + "\n");
		}
	}
	void declare_function(std::string name, std::string ret, int line, bool def=false){
        bool success = false;
		std::string* words = new std::string[param_list.size()+4];
		words[0] = name; words[1] = "FUNCTION"; words[2] = ret; words[param_list.size()+3]="\n";
		for(int i=0; i<param_list.size(); i++){
			words[i+3] = param_list.at(i).first;
		}
		std::string ws[2] = {name, "ID"};
		if (def){
			std::cout << "A function is bing defined " << name << " " << ret << std::endl;
			success = symbol_table.insert_symbol(ws);
			if(!success){
				// writeIntoErrorFile("Error at Line "+std::to_string(line)+" Multiple declaration of function " + name + "\n");
			}
			declare_variables(false, line);
		} else {
			std::cout << "A function is bing declared " << name << " " << ret << std::endl;
			for(auto s:param_list){
				std::cout << "pl:ID: "<< s.second << " Declared" << std::endl;
			}
			success = symbol_table.insert_symbol(words);
			if(!success){
				writeIntoErrorFile("Error at Line "+std::to_string(line)+" Multiple declaration of function " + name + "\n");
			}
			param_list.clear();
		}
	}
	void add_to_dl(std::string id){ declared_ids.push_back(id); }
	void add_to_pl(std::string id, std::string type){ param_list.push_back({type, id}); }
}


start : program
      ;

program : program unit
        | unit
        ;

unit : var_declaration
        | func_declaration
        | func_definition
        ;

func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
        | type_specifier ID LPAREN RPAREN SEMICOLON
        ;

func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement
    | type_specifier ID LPAREN RPAREN compound_statement
    ;

parameter_list : parameter_list COMMA type_specifier ID
        | parameter_list COMMA type_specifier
        | type_specifier ID
        | type_specifier
        ;

compound_statement : LCURL statements RCURL
        | LCURL RCURL
        ;

var_declaration 
        : ts=type_specifier { global_type=$ts.read; } dl=declaration_list SEMICOLON
        ;

type_specifier returns [std::string read]
        : INT       { $read = $INT->getText(); }
        // | FLOAT
        | VOID      { $read = $VOID->getText(); }
        | CHAR      { $read = $CHAR->getText(); }
        ;
declaration_list 
        : declaration_list COMMA ID
        | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
        | ID
        | ID LTHIRD CONST_INT RTHIRD
        ;

statements : statement
        | statements statement
        ;

statement : var_declaration
        | expression_statement
        | compound_statement
        | FOR LPAREN expression_statement expression_statement expression RPAREN statement
        | IF LPAREN expression RPAREN statement
        | IF LPAREN expression RPAREN statement ELSE statement
        | WHILE LPAREN expression RPAREN statement
        | PRINTLN LPAREN ID RPAREN SEMICOLON
        | RETURN expression SEMICOLON
        ;
expression_statement : SEMICOLON
        | expression SEMICOLON
        ;
variable : ID
        | ID LTHIRD expression RTHIRD
        ;
expression : logic_expression
        | variable ASSIGNOP logic_expression
        ;
logic_expression : rel_expression
        | rel_expression LOGICOP rel_expression
        ;
        

rel_expression : simple_expression
        | simple_expression RELOP simple_expression
        ;
simple_expression : term
        | simple_expression ADDOP term
        ;
term : unary_expression
        | term MULOP unary_expression
        ;
unary_expression : ADDOP unary_expression
        | NOT unary_expression
        | factor
        ;

factor : variable
        | ID LPAREN argument_list RPAREN
        | LPAREN expression RPAREN
        | CONST_INT
        | CONST_FLOAT
        | variable INCOP
        | variable DECOP
        ;
argument_list : arguments
        ;
arguments : arguments COMMA logic_expression
        | logic_expression
        ;