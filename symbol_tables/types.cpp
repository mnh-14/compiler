#include "types.hpp"

Type::Type(std::string t, int s): type_string(t), size(s) {}

int Type::get_size() { return this->size; }

Type::~Type() {}

IntType::IntType(): Type("INT", 2) {}

CharType::CharType(): Type("CHAR", 1) {}