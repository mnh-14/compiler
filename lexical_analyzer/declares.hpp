#ifndef DECLARES_HPP
#define DECLARES_HPP

#include <string>

class declared{
public:
    static std::string comment;
    static std::string string_lex;
    


};

class tokens{
public:
    inline static const std::string IF = "<IF>";
    inline static const std::string ELSE = "<ELSE>";
    inline static const std::string DO = "<DO>";
    inline static const std::string INT = "<INT>";
    inline static const std::string FLOAT = "<FLOAT>";
    inline static const std::string VOID = "<VOID>";
    inline static const std::string SWITCH = "<SWITCH>";
    inline static const std::string DEFAULT = "<DEFAULT>";
    inline static const std::string WHILE = "<WHILE>";
    inline static const std::string BREAK = "<BREAK>";
    inline static const std::string CHAR = "<CHAR>";
    inline static const std::string DOUBLE = "<DOUBLE>";
    inline static const std::string RETURN = "<RETURN>";
    inline static const std::string CASE = "<CASE>";
    inline static const std::string CONTINUE = "<CONTINUE>";
    inline static const std::string GOTO = "<GOTO>";
    inline static const std::string LONG = "<LONG>";
    inline static const std::string SHORT = "<SHORT>";
    inline static const std::string STATIC = "<STATIC>";
    inline static const std::string UNSIGNED = "<UNSIGNED>";

    inline static const std::string ADDOP = "ADDOP";
    inline static const std::string MULOP = "MULOP";
    inline static const std::string INCOP = "INCOP";
    inline static const std::string RELOP = "RELOP";
    inline static const std::string ASSIGNOP = "ASSIGNOP";
    inline static const std::string LOGICOP = "LOGICOP";
    inline static const std::string NOT = "NOT";
    inline static const std::string LPAREN = "LPAREN";
    inline static const std::string RPAREN = "RPAREN";
    inline static const std::string LCURL = "LCURL";
    inline static const std::string RCURL = "RCURL";
    inline static const std::string LTHIRD = "LTHIRD";
    inline static const std::string RTHIRD = "RTHIRD";
    inline static const std::string COMMA = "COMMA";
    inline static const std::string SEMICOLON = "SEMICOLON";

    
    static std::string construct_token(std::string name, std::string attribute){
        return "<"+name+", "+attribute+">";
    }
    
};


#endif