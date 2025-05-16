%option c++
%option noyywrap

%{
    #include <iostream>
    #include <string>
    #include <fstream>
    #include "declares.hpp"
    #include "token_handler.hpp"
    using namespace std;

    SymbolTable declared::symbol_table(7);

%}

VARIABLE [a-zA-Z_]+[a-zA-Z_0-9]*
%x COMMENT
%x STRING
SINGLE_COMMENT //.*(\n|(\\\n.*)+)

%%

"/*" { BEGIN(COMMENT); declared::comment=""; }
<COMMENT>"*/" { BEGIN(INITIAL); cout << "Found comment: " << declared::comment << endl; }
<COMMENT>[^*]+ { declared::comment += yytext; }
<COMMENT>"*" { declared::comment += yytext; }

"if"        { handle_no_attribute_token(tokens::IF, yytext); }
"else"      { handle_no_attribute_token(tokens::ELSE, yytext); }
"do"        { handle_no_attribute_token(tokens::DO, yytext); }
"int"       { handle_no_attribute_token(tokens::INT, yytext); }
"float"     { handle_no_attribute_token(tokens::FLOAT, yytext); }
"void"      { handle_no_attribute_token(tokens::VOID, yytext); }
"switch"    { handle_no_attribute_token(tokens::SWITCH, yytext); }
"default"   { handle_no_attribute_token(tokens::DEFAULT, yytext); }
"while"     { handle_no_attribute_token(tokens::WHILE, yytext); }
"break"     { handle_no_attribute_token(tokens::BREAK, yytext); }
"char"      { handle_no_attribute_token(tokens::CHAR, yytext); }
"double"    { handle_no_attribute_token(tokens::DOUBLE, yytext); }
"return"    { handle_no_attribute_token(tokens::RETURN, yytext); }
"case"      { handle_no_attribute_token(tokens::CASE, yytext); }
"continue"  { handle_no_attribute_token(tokens::CONTINUE, yytext); }
"goto"      { handle_no_attribute_token(tokens::GOTO, yytext); }
"long"      { handle_no_attribute_token(tokens::LONG, yytext); }
"unsigned"  { handle_no_attribute_token(tokens::UNSIGNED, yytext); }

("++"|"--") { handle_attributed_token(tokens::INCOP, yytext); }

[+-]        { handle_attributed_token(tokens::ADDOP, yytext); }
[*/%]       { handle_attributed_token(tokens::MULOP, yytext); }

("<="|">=") { handle_attributed_token(tokens::RELOP, yytext); }
("=="|"!=") { handle_attributed_token(tokens::RELOP, yytext); }
[<>]        { handle_attributed_token(tokens::RELOP, yytext); }
"="         { handle_attributed_token(tokens::ASSIGNOP, yytext); }

("&&"|"||") { handle_attributed_token(tokens::LOGICOP, yytext); }
"!"         { handle_attributed_token(tokens::NOT, yytext); }

[()]        { handle_attributed_token(tokens::LPAREN, yytext); }
[{}]        { handle_attributed_token(tokens::LCURL, yytext); }
"["         { handle_attributed_token(tokens::LTHIRD, yytext); }
"]"         { handle_attributed_token(tokens::RTHIRD, yytext); }

","         { handle_attributed_token(tokens::COMMA, yytext); }
";"         { handle_attributed_token(tokens::SEMICOLON, yytext); }

{VARIABLE}  { handle_attributed_token(tokens::ID, yytext, true); }

\n          { declared::line_count++; }


. {/* ignore */}


%%

int main(int argc, char *argv[]){
    string fin, log_out="log_result.txt", token_out="tokens.txt";

    switch(argc){
        case 4:
            log_out = argv[3];
        case 3:
            token_out = argv[2];
        case 2:
            fin = argv[1]; break;
        default:
            cerr << "Required format: <file-to-scan> [optionals]: <token-output-file> <log-output-file>";
    }

	/* FILE *fin=fopen(argv[1],"r"); */
    declared::input_file.open(fin);
    if(!declared::input_file.is_open()) cerr << "Failed to open \"" << fin << "\" file" << endl;
    declared::log_file.open(log_out);
    if(!declared::log_file.is_open()) cerr << "Failed to open \"" << log_out << "\" file" << endl;
    declared::token_file.open(token_out);
    if(!declared::token_file.is_open()) cerr << "Failed to open \"" << token_out << "\" file" << endl;
    
    declared::comment = "";
    declared::string_lexeme = "";
    
    yyFlexLexer lexer(&declared::input_file);
    
    while(true){
        if(lexer.yylex() == 0)
            break;        
    }
    
    return 0;

}