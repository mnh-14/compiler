#ifndef TOKEN_HANDLER_HPP
#define TOKEN_HANDLER_HPP

#include <string>
#include "declares.hpp"
#include "../symbol_tables/symbol_table.hpp"
#include "../symbol_tables/tools.hpp"



void report_to_log(std::string token, std::string lexeme, int line_no){
    declared::log_file << "Line no " << line_no << ": Token " << token << " Lexeme " << lexeme << " found" << std::endl << std::endl;
}

void handle_no_attribute_token(std::string token, std::string lexeme){
    declared::token_file << token << ' ';
    // declared::log_file << "Line no " << declared::line_count << ": Token " << token << " Lexeme " << lexeme << " found" << std::endl << std::endl;
    report_to_log(token, lexeme, declared::line_count);
}

std::string handle_attributed_token(std::string token_type, std::string lexeme, bool insert_symbol=false){
    std::string token = tokens::construct(token_type, lexeme);
    declared::token_file << token << " ";
    // declared::log_file << "Line no " << declared::line_count << ": " << tokens::log_string(token_type, lexeme) << " found" << std::endl << std::endl;
    report_to_log("<"+token_type+">", lexeme, declared::line_count);
    
    if(insert_symbol){
        std::string info[2] = {lexeme, token_type};
        declared::symbol_table.insert_symbol(info);
        declared::log_file << "A symbol has been inserted" << std::endl << std::endl;
    }
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
        default:  special = ""; break;
    }
    return special;
}


bool handle_string_escape_characters(char c){
    std::string special = escape_characters(c);
    if(special==""){
        std::cout << "An error occurred at " << declared::line_count << " FOR A STRING" << std::endl;
        return false;
    }
    declared::string_lexeme += special;
    return true;
}

void string_special_char_error(){
    std::cout << "Special character error at: " << declared::line_count << std::endl;
}

void handle_constants(std::string const_token, std::string constant){
    declared::token_file << tokens::construct(const_token, constant) << " ";
    report_to_log("<"+const_token+">", constant, declared::line_count);
}

void handle_const_escape_character(char c){
    std::string escape = escape_characters(c);
    handle_constants(tokens::CONST_CHAR, escape);
}



#endif