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

func_definition 
        : ts=type_specifier ID                    { entering_function($ID->getText(),$ts.read,$ID->getLine()); } 
                    LPAREN parameter_list RPAREN  { finalize_func_params($ID->getText()); }
                    compound_statement            { finishing_function($ID->getText()); }
        | ts=type_specifier ID                    { entering_function($ID->getText(), $ts.read, $ID->getLine()); } 
                    LPAREN RPAREN 
                    compound_statement            { finishing_function($ID->getText()); }
        ;

parameter_list 
        : parameter_list COMMA ts=type_specifier { global_type=$ts.read; } ID { insert_params_to_function($ID->getText()); }
        | parameter_list COMMA type_specifier
        | ts=type_specifier { global_type=$ts.read; }                      ID { insert_params_to_function($ID->getText()); }
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
        | FOR LPAREN expression_statement   { generate_new_tf_labels(); get_new_single_label(); place_single_label(); }
                     expression_statement   { if(tf_labels.back().first->use_count==0) jump_to_true();place_loop_label();}
                     expression             { jump_to_current_single_label(); place_true_label(); pop_single_label(); }
                     RPAREN statement       { place_jump_loop(); place_false_label(); pop_used_tf_label(); pop_loop_label(); }
        | IF LPAREN                         { generate_new_tf_labels(); } 
                    expression RPAREN       { place_true_label(); }
                    statement               { jump_to(get_new_single_label()); } 
                    ELSE                    { place_false_label(); pop_used_tf_label(); } 
                    statement               { place_single_label(); pop_single_label(); }
        | IF LPAREN                         { generate_new_tf_labels(); }
                    expression RPAREN       { place_true_label(); } 
                    statement               { place_false_label(); pop_used_tf_label(); }
        | WHILE LPAREN                      { generate_new_tf_labels(); place_loop_label(); } 
                    e=expression RPAREN     { simple_to_conditionals($e.is_simple); place_true_label(); } 
                    statement               { place_jump_loop(); place_false_label(); pop_used_tf_label(); pop_loop_label(); }
        | PRINTLN LPAREN ID RPAREN SEMICOLON{ 
                                            move("AX", get_memloc($ID->getText()));
                                            codeblock << INDENT << "CALL print_output" << std::endl;
                                        }
        | RETURN expression SEMICOLON       { return_statement(); }
        | ID LPAREN argument_list RPAREN    { codeblock << INDENT << "CALL " << $ID->getText() << std::endl; }
        ;
expression_statement returns [bool is_simple]
        : SEMICOLON
        | e=expression SEMICOLON { $is_simple = $e.is_simple; }
        ;

variable returns [std::string mem, std::string id]
        : ID	{ $mem = get_memloc($ID->getText()); $id=$ID->getText(); }
        | ID LTHIRD expression RTHIRD
        ;
expression returns [bool is_simple]
        : le=logic_expression { $is_simple = $le.is_simple; }
        | v=variable ASSIGNOP { generate_new_tf_labels(); } le=logic_expression { write_assign_asmcode($v.mem, $le.is_simple); pop_used_tf_label(); $is_simple=true;}
        ;
logic_expression returns [bool is_simple]
        : re=rel_expression   { $is_simple=$re.is_simple; }
        | { set_jumpable(false, true); } re=rel_expression              { simple_to_conditionals($re.is_simple); } 
                                            ANDOP re2=rel_expression    { simple_to_conditionals($re2.is_simple); $is_simple = false; tf_jumpable.pop(); } // Need to handle this for simple expressions
        | { set_jumpable(true, false); } re=rel_expression              { simple_to_conditionals($re.is_simple); }
                                            OROP re2=rel_expression     { simple_to_conditionals($re2.is_simple); $is_simple = false; jump_to_false(); tf_jumpable.pop(); }
        ;
        

rel_expression returns [bool is_simple]
        : simple_expression { $is_simple = true; }
        | simple_expression { move("CX", "AX", true);  } RELOP simple_expression   { $is_simple = false; compare_asmcode($RELOP->getText(), true); }
        ;
simple_expression 
        : term 
        | simple_expression { move("CX", "AX", true); } ADDOP term { addop_asmcode($ADDOP->getText()); pop_asmcode("CX"); }
        ;
term : unary_expression
        | term { move("BX", "AX", true); } MULOP unary_expression  { mulop_asmcode($MULOP->getText()); pop_asmcode("BX"); }
        ;
unary_expression 
        : ADDOP unary_expression { negate_operation($ADDOP->getText(), "AX"); }
        | NOT unary_expression
        | factor
        ;


factor 
    : v=variable       { move("AX", $v.mem); }
    | ID LPAREN argument_list RPAREN { codeblock << INDENT << "CALL " << $ID->getText() << std::endl; }
    | LPAREN expression RPAREN
    | CONST_INT		   { codeblock << INDENT << "MOV AX, " << $CONST_INT->getText() << std::endl;  }
    | CONST_FLOAT
    | v=variable INCOP { move("AX", $v.mem); codeblock << INDENT << "INC WORD " << $v.mem << std::endl; }
    | v=variable DECOP { move("AX", $v.mem); codeblock << INDENT << "DEC WORD " << $v.mem << std::endl; }
    ;
    

argument_list 
        : arguments
        |
        ;
arguments 
        : arguments COMMA logic_expression { push_asmcode("AX"); }
        | logic_expression { push_asmcode("AX"); }
        ;