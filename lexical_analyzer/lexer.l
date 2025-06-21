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
MAL_VAR [0-9]+[a-zA-Z_][a-zA-Z_0-9]*
NUM [0-9]
FLOAT {NUM}*\.?{NUM}*(E[+-]?{NUM}+)?
ILL_FLOAT {NUM}*\.?{NUM}+(E[+-]?({NUM}|\.)+)?
MUL_FLOAT {NUM}*\.(\.|{NUM})+(E[+-]?{NUM}+)?

S_CHAR \\[tnrb\\'afv0]
UNF_CHAR \'(.|{S_CHAR})?[^\'\n]
UNF_CHAR_N \'(.|{S_CHAR})?\n

%x COMMENT
%x STRING
%x SCOMMENT



%%

"/*"                { BEGIN(COMMENT); declared::comment="/*"; declared::comment_height=0; }
<COMMENT>"*/"       { BEGIN(INITIAL); handle_comment_token(declared::comment+"*/"); }
<COMMENT><<EOF>>    { BEGIN(INITIAL); unfinished_comment(declared::comment); }
<COMMENT>\n         { declared::line_count++; declared::comment_height++; declared::comment+="\n"; }
<COMMENT>[^*\n]+    { declared::comment += yytext; }
<COMMENT>"*"        { declared::comment += yytext; }
"//"                { BEGIN(SCOMMENT); declared::comment="//"; declared::comment_height=0; }
<SCOMMENT>\\\n      { declared::comment += yytext; declared::line_count++; declared::comment_height++; }
<SCOMMENT>\n        { BEGIN(INITIAL); handle_comment_token(declared::comment); declared::line_count++; }
<SCOMMENT>[^\\\n]+  { declared::comment += yytext; }

"\""                { BEGIN(STRING); declared::string_log="\""; declared::string_lexeme=""; declared::string_height=0;}
<STRING>"\""        { BEGIN(INITIAL); declared::string_log+="\""; handle_string_token(declared::string_lexeme); }
<STRING>\\\n        { declared::string_log += yytext; declared::line_count++; declared::string_height++; }
<STRING>\\.         { handle_string_escape_characters(yytext[1])? : BEGIN(INITIAL); }
<STRING><<EOF>>     { BEGIN(INITIAL); unfinished_string(declared::string_log); }
<STRING>\n          { BEGIN(INITIAL); unfinished_string(declared::string_log); declared::line_count++; }
<STRING>[^\\\"\n]+  { declared::string_lexeme += yytext; declared::string_log += yytext; }


"if"        { handle_no_attribute_token(tokens::IF, yytext); }
"for"       { handle_no_attribute_token(tokens::FOR, yytext); }
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
"static"    { handle_no_attribute_token(tokens::STATIC, yytext); }
"short"     { handle_no_attribute_token(tokens::SHORT, yytext); }

("++"|"--") { handle_attributed_token(tokens::INCOP, yytext); }

[+-]        { handle_attributed_token(tokens::ADDOP, yytext); }
[*/%]       { handle_attributed_token(tokens::MULOP, yytext); }

("<="|">=") { handle_attributed_token(tokens::RELOP, yytext); }
("=="|"!=") { handle_attributed_token(tokens::RELOP, yytext); }
[<>]        { handle_attributed_token(tokens::RELOP, yytext); }
"="         { handle_attributed_token(tokens::ASSIGNOP, yytext); }

("&&"|"||") { handle_attributed_token(tokens::LOGICOP, yytext); }
"!"         { handle_attributed_token(tokens::NOT, yytext); }

"("         { handle_attributed_token(tokens::LPAREN, yytext); }
")"         { handle_attributed_token(tokens::RPAREN, yytext); }
"{"         { handle_attributed_token(tokens::LCURL, yytext); declared::symbol_table.enter_new_scope(); }
"}"         { handle_attributed_token(tokens::RCURL, yytext); declared::symbol_table.exit_scope(); }
"["         { handle_attributed_token(tokens::LTHIRD, yytext); }
"]"         { handle_attributed_token(tokens::RTHIRD, yytext); }

","         { handle_attributed_token(tokens::COMMA, yytext); }
";"         { handle_attributed_token(tokens::SEMICOLON, yytext); }

{VARIABLE}  { handle_attributed_token(tokens::ID, yytext, true); }
{MAL_VAR}   { invalied_identifier(yytext); }

\'\'            { empty_character(); }
\'\'\'          { cout << "Got Error: " << yytext[1] << endl;}
\'.\'           { handle_constants(tokens::CONST_CHAR, string(1, yytext[1]), true); }
\'{S_CHAR}\'    { handle_const_escape_character(yytext[2], true); }
{UNF_CHAR}      { unfinished_character(yytext); }
{UNF_CHAR_N}    { unfinished_character(yytext); declared::line_count++; }
\'..+\'         { multi_character(yytext); }


{NUM}+      { handle_constants(tokens::CONST_INT, yytext, true); }
{FLOAT}     { handle_constants(tokens::CONST_FLOAT, yytext, true); }
{MUL_FLOAT} { too_many_decimal_point(yytext); }
{ILL_FLOAT} { ill_formed_number(yytext); }

[#@$]     { unrecognized_character(yytext); }


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
    
    declared::log_file << declared::symbol_table.all_scope_string() << std::endl;
    declared::log_file << "Total lines: " << declared::line_count << endl;
    declared::log_file << "Total errors: " << declared::error_count << endl;
    
    
    return 0;

}