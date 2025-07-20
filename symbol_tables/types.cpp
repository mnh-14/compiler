#include "types.hpp"

Type::Type(std::string t, int s): type_string(t), size(s) {}

Type::Type(){}

int Type::get_size() { return this->size; }

Type::~Type() {}

IntType::IntType(): Type("INT", 2) {}

CharType::CharType(): Type("CHAR", 1) {}

MemLocation::MemLocation(std::string bp, int off): base_ptr(bp), offset(off){}

MemLocation::MemLocation() {}

std::string MemLocation::get_location() {
    std::string off;
    if(offset >= 0) off = "+" + std::to_string(offset);
    else off = std::to_string(offset);
    return "["+base_ptr + off + "]";
}


std::string GlobalMemLocation::get_location() { return base_ptr; }

GlobalMemLocation::GlobalMemLocation(): MemLocation() {}

GlobalMemLocation::GlobalMemLocation(std::string name): MemLocation() { base_ptr = name; }

Type * Type::construct_type(std::string t){
    if(t == "int") return new IntType();
    else if(t=="char") return new CharType();
    return nullptr;
}
