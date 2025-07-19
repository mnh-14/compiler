#include "types.hpp"

Type::Type(std::string t, int s): type_string(t), size(s) {}

Type::Type(){}

int Type::get_size() { return this->size; }

Type::~Type() {}

IntType::IntType(): Type("INT", 2) {}

CharType::CharType(): Type("CHAR", 1) {}

MemLocation::MemLocation(std::string bp, int off): base_ptr(bp), offset(off){}

std::string MemLocation::get_location() { return "["+base_ptr + std::to_string(offset) + "]"; }