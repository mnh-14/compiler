#ifndef TOKEN_HANDLER_HPP
#define TOKEN_HANDLER_HPP

#include <string>
#include "declares.hpp"
#include "../symbol_tables/symbol_table.hpp"
#include "../symbol_tables/tools.hpp"




// Error handlers start
void error_report_to_log(std::string report, int line_no){
    declared::error_count++;
    declared::log_file << "Error at line no " << line_no << ": " << report << std::endl << std::endl;
}


void too_many_decimal_point(std::string sequence){
    error_report_to_log("Too many decimal points "+sequence, declared::line_count);
}

void ill_formed_number(std::string sequence){
    error_report_to_log("Ill formed number "+sequence, declared::line_count);
}

void invalied_identifier(std::string sequence){
    error_report_to_log("Invalid prefix on ID or invalid suffix on Number "+sequence, declared::line_count);
}

void multi_character(std::string sequence){
    error_report_to_log("Multi character constant error "+sequence, declared::line_count);
}

void unfinished_character(std::string sequence){
    error_report_to_log("Unterminated character "+sequence, declared::line_count);
}

void empty_character(){
    error_report_to_log("Empty character constant error ''", declared::line_count);
}

void unfinished_string(std::string sequence){
    error_report_to_log("Unterminated string "+sequence, declared::line_count-declared::string_height);
}

void unfinished_comment(std::string sequence){
    error_report_to_log("Unterminaterd comment "+sequence, declared::line_count-declared::comment_height);
}

void unrecognized_character(char c){
    error_report_to_log("Unrecognized character"+c, declared::line_count);
}
// Error Handlers end







void insert_to_symbol_table(std::string token_type, std::string lexeme){
    std::string info[2] = {lexeme, token_type};
    declared::symbol_table.insert_symbol(info);
    declared::log_file << "A symbol has been inserted" << std::endl << std::endl;
}

void report_to_log(std::string token, std::string lexeme, int line_no){
    declared::log_file << "Line no " << line_no << ": Token " << token << " Lexeme " << lexeme << " found" << std::endl << std::endl;
}

void handle_no_attribute_token(std::string token, std::string lexeme){
    declared::token_file << token << ' ';
    // declared::log_file << "Line no " << declared::line_count << ": Token " << token << " Lexeme " << lexeme << " found" << std::endl << std::endl;
    report_to_log(token, lexeme, declared::line_count);
}

std::string handle_attributed_token(std::string token_type, std::string lexeme, bool insert_symbol=false, std::string inserting_lexeme=""){
    std::string token = tokens::construct(token_type, lexeme);
    declared::token_file << token << " ";
    // declared::log_file << "Line no " << declared::line_count << ": " << tokens::log_string(token_type, lexeme) << " found" << std::endl << std::endl;
    report_to_log("<"+token_type+">", lexeme, declared::line_count);
    
    if(insert_symbol)
        inserting_lexeme==""? insert_to_symbol_table(token_type, lexeme) : insert_to_symbol_table(token_type, inserting_lexeme);
    return token;
}


void handle_comment_token(std::string comment){
    report_to_log(tokens::COMMENT, comment, declared::line_count-declared::comment_height);
}



void handle_string_token(std::string lexeme){
    declared::token_file << tokens::construct(tokens::STRING, lexeme) << " ";
    report_to_log("<"+tokens::STRING+">", lexeme, declared::line_count-declared::string_height);
}


std::string escape_characters(char c){
    std::string special="";
    switch(c){
        case 'n': special = "\n"; break;
        case 't': special = "\t"; break;
        case '\\': special = "\\"; break;
        case '\'': special = "\'"; break;
        case 'a': special = "\a"; break;
        case 'f': special = "\f"; break;
        case 'r': special = "\r"; break;
        case 'b': special = "\b"; break;
        case 'v': special = "\v"; break;
        case '0': special = "\0"; break;
        case '\"': special = "\""; break;
        default:  special = ""; break;
    }
    return special;
}


bool handle_string_escape_characters(char c){
    std::string special = escape_characters(c);
    if(special==""){
        error_report_to_log("Unrecognized special character "+declared::string_lexeme+"\\"+c, declared::line_count-declared::string_height);
        return false;
    }
    declared::string_lexeme += special;
    return true;
}

void string_special_char_error(){
    std::cout << "Special character error at: " << declared::line_count << std::endl;
}

void handle_constants(std::string const_token, std::string constant, bool insert_symbol=false, std::string inserting_lexeme=""){
    declared::token_file << tokens::construct(const_token, constant) << " ";
    report_to_log("<"+const_token+">", constant, declared::line_count);
    if(insert_symbol)
        inserting_lexeme==""? insert_to_symbol_table(const_token, constant) : insert_to_symbol_table(const_token, inserting_lexeme);
}

void handle_const_escape_character(char c, bool insert_symbol=false){
    std::string escape = escape_characters(c);
    if(escape==""){
        error_report_to_log("Undefined special character '\\"+c, declared::line_count);
        return;
    }
    handle_constants(tokens::CONST_CHAR, escape, insert_symbol, "'\\"+escape+"'");
}





#endif