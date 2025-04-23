#ifndef SYMBOL_INFO_HPP
#define SYMBOL_INFO_HPP


#include <string>
#include <iostream>
#include "./tools.hpp"



class SymbolInfo {
private:
    std::string symbol;
    std::string symbol_type;
    SymbolInfo* next_symbolinfo = nullptr;
public:
    SymbolInfo(std::string symbol, std::string symbol_type);
    SymbolInfo(){}
    void set_symbol(std::string symbol){ this->symbol = symbol; }
    void set_symbol_type(std::string stype){ this->symbol_type = stype; }
    std::string get_symbol(){ return this->symbol; }
    std::string get_symbol_type(){ return this->symbol_type; }
    void set_next_symbolinfo(SymbolInfo* s_info){ this->next_symbolinfo = s_info; }
    SymbolInfo* get_next_symbolinfo(){ return this->next_symbolinfo; }
    virtual std::string get_string_repr(std::string lbrace, std::string rbrace);
    virtual std::string get_string_repr();
    virtual ~SymbolInfo();
};



SymbolInfo::SymbolInfo(std::string symbol, std::string symbol_type){
    this->symbol = symbol;
    this->symbol_type = symbol_type;
}

SymbolInfo::~SymbolInfo(){
}

std::string SymbolInfo::get_string_repr(std::string lbrace, std::string rbrace){
    return lbrace + this->symbol + "," + this->symbol_type + rbrace;
}

std::string SymbolInfo::get_string_repr(){
    return this->get_string_repr("<", ">");
}


// A new Function Symbol Info class to store info on Function symbols
class FunctionSymbolInfo: public SymbolInfo{
private:
    std::string return_type;
    std::string* argument_types = nullptr;
public:
    FunctionSymbolInfo(std::string symbol, std::string symbol_type, std::string rtype, std::string* arguments);
    FunctionSymbolInfo(std::string symbol, std::string symbol_type, std::string rtype);
    void set_arguments(std::string* args);
    FunctionSymbolInfo(){}
    ~FunctionSymbolInfo();
    std::string get_string_repr(std::string lbrace, std::string rbrace) override;
};


FunctionSymbolInfo::FunctionSymbolInfo(std::string symbol, std::string symbol_type,
                                        std::string rtype, std::string* arguments)
                    : SymbolInfo(symbol, symbol_type)
{
    this->return_type = rtype;
    this->set_arguments(arguments);
}


void FunctionSymbolInfo::set_arguments(std::string* arguments){
    int arg_count = 0;
    for(;;arg_count++){
        if (arguments[arg_count]=="\n")
            break;
    }
    if(arg_count==0)
        return;
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
// End of Function Symbol Info class
// 


template <typename TYPE>
class SimplePair{
private:
    TYPE first, second;
public:
    SimplePair(std::string str1, std::string str2){
        first = str1;
        second = str2;
    }
    SimplePair(){}
    TYPE getfirst(){ return first; }
    TYPE getsecond(){ return second; }
    
};

// class StructSymbolInfo: public SymbolInfo{
// private:
//     SymbolInfo** s_infos = nullptr;
//     SimplePair<std::string> ** strucsymbols;

// public:
//     StructSymbolInfo(std::string symbol, std::string symbol_type, std::string* symbols);
//     StructSymbolInfo(std::string symbol, std::string symbol_type);
//     StructSymbolInfo(){}
//     void set_struc_symbols(std::string * symbols);
//     ~StructSymbolInfo();
//     std::string get_string_repr(std::string lbrace, std::string rbrace) override;
// };

// StructSymbolInfo::StructSymbolInfo(std::string symbol, std::string symbol_type, std::string* symbols): SymbolInfo(symbol, symbol_type){
//     this->set_struc_symbols(symbols);
// }

// StructSymbolInfo::StructSymbolInfo(std::string symbol, std::string symbol_type): SymbolInfo(symbol, symbol_type){

// }

// void StructSymbolInfo::set_struc_symbols(std::string* symbols){
//     int s_count = 0;
//     for(; ; s_count++){
//         if (symbols[s_count]=="\n")
//             break;
//     }

//     if(s_count==0)
//         return;
//     if(s_count & 1 == 1){
//         throw std::invalid_argument("For STRUC, each symbol must have it's symbol type");
//     }

//     s_count = s_count / 2;
//     s_infos = new SymbolInfo*[s_count+1];
//     strucsymbols = new SimplePair<std::string>*[s_count+1];
//     for(int i=0, idx=0; ; idx++, i+=2){
//         if(symbols[i]=="\n")
//             break;
//         s_infos[idx] = new SymbolInfo(symbols[i], symbols[i+1]);
//         strucsymbols[idx] = new SimplePair<std::string>(symbols[i], symbols[i+1]);
//     }
//     s_infos[s_count] = nullptr;
//     strucsymbols[s_count] = nullptr;
// }

// // StructSymbolInfo::~StructSymbolInfo(){
// //     if (s_infos==nullptr)
// //         return;
// //     for(int i=0; ; i++){
// //         if(s_infos[i]==nullptr){
// //             delete s_infos[i];
// //             break;
// //         }
// //         delete s_infos[i];
// //     }
// //     delete[] s_infos;
// // }

// StructSymbolInfo::~StructSymbolInfo(){
//     if (strucsymbols==nullptr)
//         return;
//     for(int i=0; ; i++){
//         if(strucsymbols[i]==nullptr){
//             delete strucsymbols[i];
//             break;
//         }
//         delete strucsymbols[i];
//     }
//     delete[] strucsymbols;
// }

// std::string StructSymbolInfo::get_string_repr(std::string lbrace, std::string rbrace){
//     std::string symbols = "";
//     for(int i=0; s_infos!=nullptr; i++){
//         if(s_infos[i+1]==nullptr){
//             symbols = symbols + s_infos[i]->get_string_repr("(", ")");
//             break;
//         }
//         symbols = symbols + s_infos[i]->get_string_repr("(", ")") + ",";
//     }    
    
//     return lbrace + this->get_symbol() + "," + this->get_symbol_type() + ",{" + symbols + "}" + rbrace;
// }
//
// std::string StructSymbolInfo::get_string_repr(std::string lbrace, std::string rbrace){
//     std::string symbols = "";
//     for(int i=0; strucsymbols!=nullptr; i++){
//         if(strucsymbols[i+1]==nullptr){
//             // symbols = symbols + strucsymbols[i]->get_string_repr("(", ")");
//             symbols = symbols + "(" + strucsymbols[i]->getfirst() + "," + strucsymbols[i]->getsecond() + ")";
//             break;
//         }
//         // symbols = symbols + strucsymbols[i]->get_string_repr("(", ")") + ",";
//         symbols = symbols + "(" + strucsymbols[i]->getfirst() + "," + strucsymbols[i]->getsecond() + ")" + ",";
//     }    
    
//     return lbrace + this->get_symbol() + "," + this->get_symbol_type() + ",{" + symbols + "}" + rbrace;
// }
// Structure Symbol Info
class StructSymbolInfo: public SymbolInfo{
private:
    SimplePair<std::string> ** strucsymbols;

public:
    StructSymbolInfo(std::string symbol, std::string symbol_type, std::string* symbols);
    StructSymbolInfo(std::string symbol, std::string symbol_type);
    StructSymbolInfo(){}
    void set_struc_symbols(std::string * symbols);
    ~StructSymbolInfo();
    std::string get_string_repr(std::string lbrace, std::string rbrace) override;
};

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




#endif