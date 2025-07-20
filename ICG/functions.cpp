#include "functions.hpp"

using namespace std;

void insert_variable_to_symbletable_and_gencode(string vid) {
    string parts[] = {vid, "VAR"};
    auto si = symbol_table.insert_symbol(parts);
    Type * t = Type::construct_type(global_type);
    si->set_type(t);
    if (symbol_table.is_global_scope()){
        si->set_memeory(new GlobalMemLocation(vid));
        asmfile << INDENT << vid << " " << definitions[t->size] << " 0" << endl;
    }
    else{
        int off = symbol_table.get_space_for_local_variable(t);
        si->set_memeory(new MemLocation("bp", off));
        codeblock << INDENT << "SUB SP, " << t->size << endl; 
    }
}

void forced_enter_scope(){
    symbol_table.enter_new_scope();
    is_new_scoped=true;
}
void enter_scope(){
    if(is_new_scoped)
        is_new_scoped=false;
    else
        symbol_table.enter_new_scope();
}
void exit_scope(){
    symbol_table.exit_scope();
}

void anything(){
    string mem;

    mem = symbol_table.lookup("am")->get_memory()->get_location();
    codeblock << "MOV AX, " << endl;
}

void entering_function(string fid, string rettype, int line_no){
    FunctionSymbolInfo* si;
    si = dynamic_cast<FunctionSymbolInfo*>(symbol_table.lookup(fid));
    if(!si){
        string parts[] = {fid, "FUNCTION", rettype, "\n"};
        si = dynamic_cast<FunctionSymbolInfo*>(symbol_table.insert_symbol(parts));
    }
    forced_enter_scope();
    codeblock << fid << " PROC" << INDENT << INDENT << "; " << line_no << " no. line" << endl;
    codeblock << INDENT << "PUSH BP" << endl;
    codeblock << INDENT << "MOV BP, SP" << endl;
}

void finishing_function(string fid){
    codeblock << "RET " << 2 << endl;
    codeblock << "ENDP " << fid << endl;
}

