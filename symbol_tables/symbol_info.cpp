#include <string>
#include <iostream>
#include "./tools.hpp"
#include "./symbol_info.hpp"



SymbolInfo::SymbolInfo(){}

void SymbolInfo::set_symbol(std::string symbol){ this->symbol = symbol; }

void SymbolInfo::set_symbol_type(std::string stype){ this->symbol_type = stype; }

std::string SymbolInfo::get_symbol(){ return this->symbol; }

std::string SymbolInfo::get_symbol_type(){ return this->symbol_type; }

void SymbolInfo::set_next_symbolinfo(SymbolInfo* s_info){ this->next_symbolinfo = s_info; }

SymbolInfo* SymbolInfo::get_next_symbolinfo(){ return this->next_symbolinfo; }

void SymbolInfo::set_type(Type * t) { this->info_type = t; }

Type* SymbolInfo::get_type() { return this->info_type; }

void SymbolInfo::set_memeory(MemLocation *ml) { this->memlocation = ml; }

MemLocation* SymbolInfo::get_memory() { return this->memlocation; }

void FunctionSymbolInfo::set_arg_byte_size(int sz) { this->arg_byte_size = sz; }
int FunctionSymbolInfo::get_arg_byte_size() { return this->arg_byte_size; }

SymbolInfo::SymbolInfo(std::string symbol, std::string symbol_type){
    this->symbol = symbol;
    this->symbol_type = symbol_type;
}

SymbolInfo::~SymbolInfo(){
    delete info_type;
}

std::string SymbolInfo::get_string_repr(std::string lbrace, std::string rbrace){
    return lbrace + this->symbol + " : " + this->symbol_type + rbrace;
}

std::string SymbolInfo::get_string_repr(){
    return this->get_string_repr("< ", " >");
}




// FunctionsymbolInfo class methods
FunctionSymbolInfo::FunctionSymbolInfo(){}

FunctionSymbolInfo::FunctionSymbolInfo(std::string symbol, std::string symbol_type,
                                        std::string rtype, std::string* arguments)
                    : SymbolInfo(symbol, symbol_type)
{
    this->return_type = rtype;
    this->set_arguments(arguments);
    arg_clean_required = false;
}


void FunctionSymbolInfo::set_arguments(std::string* arguments){
    int arg_count = 0;
    for(;;arg_count++){
        if (arguments[arg_count]=="\n")
            break;
    }
    if(arg_count==0)
        return;
    arg_clean_required = true;
    // std::cout << "DID I GET It RIGHT line 64" << std::endl;
    argument_types = new std::string[arg_count+1];
    for(int i=0; ; i++){
        argument_types[i] = arguments[i];
        if (arguments[i] == "\n")
            break;
    }
}

FunctionSymbolInfo::FunctionSymbolInfo(std::string symbol, std::string symbol_type, std::string rtype)
                    : SymbolInfo(symbol, symbol_type){

    this->return_type = rtype;
}

FunctionSymbolInfo::~FunctionSymbolInfo(){
    if(argument_types != nullptr){
        delete[] argument_types;
    }
}

std::string FunctionSymbolInfo::get_string_repr(std::string lbrace, std::string rbrace){
    std::string args = "";
    for(int i=0; argument_types!=nullptr; i++){
        if (argument_types[i+1]=="\n"){
            args = args + argument_types[i];
            break;
        }
        args = args + argument_types[i] + ",";
    }
    
    return lbrace + this->get_symbol() + "," + this->get_symbol_type() + "," + this->return_type + "<==(" + args + ")" + rbrace;
}
std::string FunctionSymbolInfo::get_return_type() { return this->return_type; }
// End of Function Symbol Info class
// 




// StructSymbol class methods
StructSymbolInfo::StructSymbolInfo(){}

StructSymbolInfo::StructSymbolInfo(std::string symbol, std::string symbol_type, std::string* symbols): SymbolInfo(symbol, symbol_type){
    this->set_struc_symbols(symbols);
}

StructSymbolInfo::StructSymbolInfo(std::string symbol, std::string symbol_type): SymbolInfo(symbol, symbol_type){

}

void StructSymbolInfo::set_struc_symbols(std::string* symbols){
    int s_count = 0;
    for(; ; s_count++){
        if (symbols[s_count]=="\n")
            break;
    }

    if(s_count==0)
        return;
    if(s_count & 1 == 1){
        throw std::invalid_argument("For STRUC, each symbol must have it's symbol type");
    }

    s_count = s_count / 2;
    strucsymbols = new SimplePair<std::string>*[s_count+1];
    for(int i=0, idx=0; ; idx++, i+=2){
        if(symbols[i]=="\n")
            break;
        strucsymbols[idx] = new SimplePair<std::string>(symbols[i], symbols[i+1]);
    }
    strucsymbols[s_count] = nullptr;
}

StructSymbolInfo::~StructSymbolInfo(){
    if (strucsymbols==nullptr)
        return;
    for(int i=0; ; i++){
        if(strucsymbols[i]==nullptr){
            delete strucsymbols[i];
            break;
        }
        delete strucsymbols[i];
    }
    delete[] strucsymbols;
}

std::string StructSymbolInfo::get_string_repr(std::string lbrace, std::string rbrace){
    std::string symbols = "";
    for(int i=0; strucsymbols!=nullptr; i++){
        if(strucsymbols[i+1]==nullptr){
            symbols = symbols + "(" + strucsymbols[i]->getfirst() + "," + strucsymbols[i]->getsecond() + ")";
            break;
        }
        symbols = symbols + "(" + strucsymbols[i]->getfirst() + "," + strucsymbols[i]->getsecond() + ")" + ",";
    }    
    
    return lbrace + this->get_symbol() + "," + this->get_symbol_type() + ",{" + symbols + "}" + rbrace;
}


