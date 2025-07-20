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
	#include "functions.hpp"
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

func_definition : ts=type_specifier ID {entering_function($ID->getText(),$ts.read,$ID->getLine());} LPAREN parameter_list RPAREN compound_statement {finishing_function($ID->getText());}
    | ts=type_specifier ID { entering_function($ID->getText(), $ts.read, $ID->getLine()); } LPAREN RPAREN compound_statement {finishing_function($ID->getText());}
    ;

parameter_list : parameter_list COMMA type_specifier { global_type=$ts.read; } ID {}
        | parameter_list COMMA type_specifier
        | ts=type_specifier { global_type=$ts.read; } ID {}
        | type_specifier
        ;

compound_statement : LCURL {enter_scope();} statements RCURL {exit_scope();}
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
        : declaration_list COMMA ID { insert_variable_to_symbletable_and_gencode($ID->getText()); }
        | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
        | ID { insert_variable_to_symbletable_and_gencode($ID->getText()); }
        | ID LTHIRD CONST_INT RTHIRD
        ;

statements : statement
        | statements statement
        ;

statement : var_declaration
        | expression_statement
        | compound_statement
        | FOR LPAREN expression_statement expression_statement expression RPAREN statement
        | IF LPAREN expression RPAREN statement ELSE statement
        | IF LPAREN expression RPAREN statement
        | WHILE LPAREN expression RPAREN statement
        | PRINTLN LPAREN ID RPAREN SEMICOLON { 
                                        move("AX", get_memloc($ID->getText()));
										codeblock << INDENT << "CALL print_output" << std::endl;
									}
        | RETURN expression SEMICOLON
        ;
expression_statement : SEMICOLON
        | expression SEMICOLON
        ;

variable returns [std::string mem]
		: ID	{ $mem = get_memloc($ID->getText()); }
        | ID LTHIRD expression RTHIRD
        ;
expression : logic_expression
        | v=variable ASSIGNOP logic_expression { move($v.mem, "AX"); }
        ;
logic_expression : rel_expression
        | rel_expression LOGICOP rel_expression
        ;
        

rel_expression
        : simple_expression
        | simple_expression RELOP simple_expression
        ;
simple_expression 
        : term 
        | simple_expression { move("CX", "AX"); } ADDOP term { addop_asmcode($ADDOP->getText()); }
        ;
term : unary_expression
        | term { move("CX", "AX"); } MULOP unary_expression { mulop_asmcode($MULOP->getText()); }
        ;
unary_expression : ADDOP unary_expression
        | NOT unary_expression
        | factor
        ;


factor 
    : v=variable   { move("AX", $v.mem); }
    | ID LPAREN argument_list RPAREN
    | LPAREN expression RPAREN
    | CONST_INT		{ codeblock << INDENT << "MOV AX, " << $CONST_INT->getText() << std::endl; }
    | CONST_FLOAT
    | v=variable INCOP { move("AX", $v.mem); codeblock << INDENT << "INC " << $v.mem << std::endl; }
    | v=variable DECOP { move("AX", $v.mem); codeblock << INDENT << "DEC " << $v.mem << std::endl; }
    ;
    

argument_list : arguments
        ;
arguments : arguments COMMA logic_expression
        | logic_expression
        ;