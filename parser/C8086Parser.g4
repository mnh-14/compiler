parser grammar C8086Parser;

options {
    tokenVocab = C8086Lexer;
}

@parser::header {
    #include <iostream>
    #include <fstream>
    #include <string>
    #include <cstdlib>
    #include "C8086Lexer.h"
	// #include "../symbol_tables/symbol_table.hpp"
	// #include "../symbol_tables/symbol_info.hpp"
    extern std::ofstream parserLogFile;
    extern std::ofstream errorFile;
    extern int syntaxErrorCount;
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


start : program {
			writeIntoparserLogFile("Parsing completed successfully with " + std::to_string(syntaxErrorCount) + " syntax errors.");
		}
	;

program returns [std::string read]
	: unit { $read=$unit.read; write_rule($unit.ctx->stop->getLine(), "program : unit", $unit.read+"\n"); }
	| p=program u=unit { $read=$p.read+"\n"+$u.read; write_rule($u.ctx->stop->getLine(), "program : program unit", $read+"\n"); }
	;
	
unit returns [std::string read]
	 : vd=var_declaration { write_rule($ctx->start->getLine(), "unit : var_declaration", $vd.read+"\n"); $read=$vd.read; }
     | fdl=func_declaration { write_rule($ctx->start->getLine(), "unit : func_declaration", $fdl.read+"\n"); $read=$fdl.read; }
     | fdf=func_definition { write_rule($ctx->start->getLine(), "unit : func_definition", $fdf.read+"\n"); $read=$fdf.read; }
     ;
     

func_declaration returns [std::string read]
		: ts=type_specifier ID LPAREN pl=parameter_list RPAREN SEMICOLON {
				$read = $ts.read + " " + $ID->getText() + "(" + $pl.read + ");";
				write_rule($ctx->start->getLine(), "func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON ", $read);
			}
		| ts=type_specifier ID LPAREN RPAREN SEMICOLON { 
				$read = $ts.read + " " + $ID->getText() + "();";
				write_rule($ctx->start->getLine(), "func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON ", $read);
			}
		;
		 

func_definition returns [std::string read]
		: ts=type_specifier ID LPAREN pl=parameter_list RPAREN cs=compound_statement {
				$read = $ts.read + " " + $ID->getText() + "(" + $pl.read + ")" + $cs.read +"\n";
				write_rule($ctx->start->getLine(), "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement ", $read);
			}
		| ts=type_specifier ID LPAREN RPAREN cs=compound_statement {
				$read = $ts.read + " " + $ID->getText() + "()" + $cs.read + "\n";
				write_rule($ctx->start->getLine(), "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement ", $read);
			}
 		;				


parameter_list returns [std::string read]
		: pl=parameter_list COMMA ts=type_specifier ID { 
				$read = $pl.read + $COMMA->getText() + $ts.read + " " + $ID->getText();
				write_rule($ctx->start->getLine(), "parameter_list : parameter_list COMMA type_specifier ID", $read); 
			}
		| pl=parameter_list COMMA ts=type_specifier { 
				$read = $pl.read + $COMMA->getText() + $ts.read;
				write_rule($ctx->start->getLine(), "parameter_list : parameter_list COMMA type_specifier", $read); 
			}
 		| ts=type_specifier ID { 
				$read = $ts.read + " " + $ID->getText();
				write_rule($ctx->start->getLine(), "parameter_list : type_specifier ID", $read); 
			}
		| ts=type_specifier { 
				$read = $ts.read;
				write_rule($ctx->start->getLine(), "parameter_list : type_specifier", $read); 
			}
 		;

 		

compound_statement returns [std::string read]
		: LCURL statements RCURL {
				$read = "{\n" + $statements.read + "\n}";
				write_rule($ctx->start->getLine(), "compound_statement : LCURL statements RCURL", $read); 
			}
		| LCURL RCURL { 
				$read = "{\n\n}";
				write_rule($ctx->start->getLine(), "compound_statement : LCURL RCURL", $read); 
			}
		;
 		    

var_declaration returns [std::string read]
    : t=type_specifier dl=declaration_list sm=SEMICOLON { 
				$read = $t.read + " " + $dl.read + ";";
				write_rule($ctx->start->getLine(), "var_declaration : type_specifier declaration_list SEMICOLON" , $read); 
			}
    | t=type_specifier de=declaration_list_err sm=SEMICOLON { 
				$read = $t.read + " ERROR";
				write_rule($ctx->start->getLine(), "var_declaration : type_specifier declaration_list_err" , $ctx->getText()); 
			}
    ;



declaration_list_err returns [std::string error_name]: {
        $error_name = "Error in declaration list";
    };

 		 
type_specifier returns [std::string read]
        : INT { write_rule($INT->getLine(), "type_specifier : INT", $INT->getText()); $read = $INT->getText(); }
 		| FLOAT { write_rule($FLOAT->getLine(), "type_specifier : FLOAT", $FLOAT->getText()); $read = $FLOAT->getText(); }
 		| VOID { write_rule($VOID->getLine(), "type_specifier : VOID", $VOID->getText()); $read = $VOID->getText(); }
 		| CHAR { write_rule($CHAR->getLine(), "type_specifier : CHAR", $CHAR->getText()); $read = $CHAR->getText(); }
 		;


declaration_list returns [std::string read]
		  : dl=declaration_list COMMA ID {
					$read = std::string($ctx->getText());
					write_rule($COMMA->getLine(), "declaration_list : declaration_list COMMA ID", $ctx->getText());
				}
 		  | dl=declaration_list COMMA ID LTHIRD CONST_INT RTHIRD {
					$read = std::string($ctx->getText());
					write_rule($COMMA->getLine(), "declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD", $ctx->getText());
				}
 		  | ID {
					$read = $ID->getText();
					write_rule($ID->getLine(), "declaration_list : ID", $ID->getText());
				}
 		  | ID LTHIRD CONST_INT RTHIRD {
					$read = $ID->getText();
					write_rule($RTHIRD->getLine(), "declaration_list : ID LTHIRD CONST_INT RTHIRD", $ctx->getText());
				}
 		  ;
 		  
statements returns [std::string read]
		: s=statement {
					$read = $s.read;
					write_rule($ctx->start->getLine(), "statements : statement", $read);
				}
		| ss=statements s=statement {
					$read = $ss.read + "\n" + $s.read;
					write_rule($ctx->start->getLine(), "statements : statements statement", $read);
				}
		;
	   


statement returns [std::string read]
		: var_declaration {
					$read = $var_declaration.read;
					write_rule($ctx->start->getLine(), "statement : var_declaration", $read);
				}
		| expression_statement {
					$read = $ctx->getText();
					write_rule($ctx->start->getLine(), "statement : expression_statement", $ctx->getText());
				}
		| cs=compound_statement {
					$read = $cs.read;
					write_rule($ctx->start->getLine(), "statement : compound_statement", $read);
				}
		| FOR LPAREN expression_statement expression_statement expression RPAREN statement {
					$read = $ctx->getText();
					write_rule($ctx->start->getLine(), 
							  "statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement",
							  $ctx->getText()
						);
				}
		| IF LPAREN expression RPAREN statement ELSE statement {
					$read = $ctx->getText();
					write_rule($ctx->start->getLine(), "statement : IF LPAREN expression RPAREN statement ELSE statement", $ctx->getText());
				}
		| IF LPAREN expression RPAREN statement {
					$read = $ctx->getText();
					write_rule($ctx->start->getLine(), "statement : IF LPAREN expression RPAREN statement", $ctx->getText());
				}
		| WHILE LPAREN expression RPAREN statement {
					$read = $ctx->getText();
					write_rule($ctx->start->getLine(), "statement : WHILE LPAREN expression RPAREN statement", $ctx->getText());
				}
		| PRINTLN LPAREN ID RPAREN SEMICOLON {
					$read = $ctx->getText();
					write_rule($ctx->start->getLine(), "statement : PRINTLN LPAREN ID RPAREN SEMICOLON", $ctx->getText());
				}
		| RETURN expression SEMICOLON {
					$read = $ctx->getText();
					write_rule($ctx->start->getLine(), "statement : RETURN expression SEMICOLON ", $ctx->getText());
				}
		;

	  
expression_statement returns [std::string read]
		: SEMICOLON {
					write_rule($ctx->start->getLine(), "expression_statement : SEMICOLON ", $ctx->getText());
				}
		| expression SEMICOLON {
					write_rule($ctx->start->getLine(), "expression_statement : expression SEMICOLON ", $ctx->getText());
				}
		;
	  
variable 
		: ID {
					write_rule($ctx->start->getLine(), "variable : ID", $ctx->getText());
				}
		| ID LTHIRD expression RTHIRD {
					write_rule($ctx->start->getLine(), "variable :ID LTHIRD expression RTHIRD", $ctx->getText());
				}
		;
	 
 expression 
 		: logic_expression	{
					write_rule($ctx->start->getLine(), "expression : logic_expression", $ctx->getText());
				}
		| variable ASSIGNOP logic_expression {
					write_rule($ctx->start->getLine(), "expression : variable ASSIGNOP logic_expression", $ctx->getText());
				}
		;
			
logic_expression 
		: rel_expression {
					write_rule($ctx->start->getLine(), "logic_expression : rel_expression", $ctx->getText());
				}
		| rel_expression LOGICOP rel_expression {
					write_rule($ctx->start->getLine(), "logic_expression : rel_expression LOGICOP rel_expression", $ctx->getText());
				}
		;
			
rel_expression	
		: simple_expression {
					write_rule($ctx->start->getLine(), "logic_expression : simple_expression", $ctx->getText());
				}
		| simple_expression RELOP simple_expression	{
					write_rule($ctx->start->getLine(), "logic_expression : simple_expression LOGICOP simple_expression", $ctx->getText());
				}
		;
				
simple_expression 
		: term {
					write_rule($ctx->start->getLine(), "simple_expressio : term", $ctx->getText());
				}
		| simple_expression ADDOP term {
					write_rule($ctx->start->getLine(), "simple_expressio : simple_expression ADDOP term", $ctx->getText());
				}
		;
					
term : unary_expression {
					write_rule($ctx->start->getLine(), "term : unary_expression", $ctx->getText());
				}
    	| term MULOP unary_expression {
					write_rule($ctx->start->getLine(), "term : term MULOP unary_expression", $ctx->getText());
				}
    	;

unary_expression
		: ADDOP unary_expression {
					write_rule($ctx->start->getLine(), "unary_expression : ADDOP unary_expression", $ctx->getText());
				}
		| NOT unary_expression {
					write_rule($ctx->start->getLine(), "unary_expression : NOT unary_expression", $ctx->getText());
				}
		| factor {
					write_rule($ctx->start->getLine(), "unary_expression : factor", $ctx->getText());
				}
		;

factor
		: variable {
					write_rule($ctx->start->getLine(), "factor : variable", $ctx->getText());
				}
		| ID LPAREN argument_list RPAREN {
					write_rule($ctx->start->getLine(), "factor : ID LPAREN argument_list RPAREN", $ctx->getText());
				}
		| LPAREN expression RPAREN {
					write_rule($ctx->start->getLine(), "factor : LPAREN expression RPAREN", $ctx->getText());
				}
		| CONST_INT {
					write_rule($ctx->start->getLine(), "factor : CONST_INT", $ctx->getText());
				}
		| CONST_FLOAT {
					write_rule($ctx->start->getLine(), "factor : CONST_FLOAT", $ctx->getText());
				}
		| variable INCOP {
					write_rule($ctx->start->getLine(), "factor : variable INCOP", $ctx->getText());
				}
		| variable DECOP {
					write_rule($ctx->start->getLine(), "factor : variable DECOP", $ctx->getText());
				}
		;

argument_list
		: arguments {
					write_rule($ctx->start->getLine(), "argument_list : arguments", $ctx->getText());
				}
		| {
				write_rule($ctx->start->getLine(), "argument_list :", $ctx->getText());
			}
		;

arguments
		: arguments COMMA logic_expression {
					write_rule($ctx->start->getLine(), "arguments : arguments COMMA logic_expression", $ctx->getText());
				}
		| logic_expression {
					write_rule($ctx->start->getLine(), "arguments : logic_expression", $ctx->getText());
				}
		;