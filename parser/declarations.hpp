#include <string>
#include <vector>
void writeIntoparserLogFile(const std::string message);
void writeIntoErrorFile(const std::string message);
void write_rule(int line_no, std::string rule, std::string matched);


namespace expr {

} // namespace expr


namespace stmnt {
    
} // namespace stmnt
  // 


namespace dlist {
    void dlist_comma_id(int line, std::vector<std::string>& vars, std::string id, std::string text);
    void dlist_comma_array(int line, std::vector<std::string>& vars, std::string id, std::string num, std::string text);
    void _id(int line, std::vector<std::string>& vars, std::string id, std::string text);
} // namespace name

