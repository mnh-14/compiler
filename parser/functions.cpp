#include <string>
#include "declarations.hpp"


void write_rule(int line_no, std::string rule, std::string matched){
    writeIntoparserLogFile("Line "+std::to_string(line_no)+": " + rule + "\n" + matched + "\n");
}


namespace dlist {
    void dlist_comma_id(int line, std::vector<std::string>& vars, std::string id, std::string text){
        vars.push_back(id);
        write_rule(line, "declartion_list : declaration_list COMMA ID", text+"");
    }
    void dlist_comma_array(int line, std::vector<std::string>& vars, std::string id, std::string num, std::string text);
    void _id(int line, std::vector<std::string>& vars, std::string id, std::string text);
} // namespace name
