#ifndef TOKEN_HANDLER_HPP
#define TOKEN_HANDLER_HPP

#include <string>
#include "declares.hpp"
#include "../symbol_tables/symbol_table.hpp"

void handle_no_attribute_token(std::string token, std::string lexeme){
    declared::token_file << token << ' ';
    declared::log_file << "Line no " << declared::line_count << ": Token " << token << " Lexeme " << lexeme << " found" << std::endl << std::endl;
}

std::string handle_attributed_token(std::string token_type, std::string lexeme, bool insert_symbol=false){
    std::string token = tokens::construct(token_type, lexeme);
    declared::token_file << token << " ";
    declared::log_file << "Line no " << declared::line_count << ": " << tokens::log_string(token_type, lexeme) << " found" << std::endl << std::endl;
    
    if(insert_symbol){
        std::string info[2] = {lexeme, token_type};
        declared::symbol_table.insert_symbol(info);
        // delete[] info;
        declared::log_file << "A symbol has been inserted" << std::endl;
    }
    return token;
}

void log_attribute_token(std::string token, std::string lexeme, int line_no){
    declared::log_file << "Line no " << line_no << ": Token " << token << " Lexeme " << lexeme << " found" << std::endl;
}


#endif