#include "functions.hpp"

using namespace std;

static int global_label_count = 0;
std::stack<pair<std::string, std::string>> tf_labels;


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
    codeblock << INDENT << "MOV SP, BP" << endl;
    codeblock << INDENT << "POP BP" << endl;
    codeblock << INDENT << "RET " << 0 << endl;
    codeblock << "ENDP " << fid << endl << endl;
}

//     switch(addop[0]){
//         case '+': return "ADD";
//         case '-': return "SUB";
//     }
//     return "?";
// }

void addop_asmcode(string addop){
    switch(addop[0]){
        case '+' : codeblock << INDENT << "ADD AX, CX" << endl; break;
        case '-' : codeblock << INDENT << "SUB CX, AX" << endl << "MOV AX, CX" << endl; break;
    }
}

string get_memloc(string id){
    return symbol_table.lookup(id)->get_memory()->get_location();
}

void move(string a, string b, int line){
    codeblock << INDENT << "MOV " << a << ", " << b << endl;
}

void mulop_asmcode(string mulop){
    switch(mulop[0]){
        case '*' :
            codeblock << INDENT << "CBW" << endl;
            codeblock << INDENT << "MUL CX" << endl;
            break;
        case '/' :
            codeblock << INDENT << "XCHG CX, AX" << endl; 
            codeblock << INDENT << "CWD" << endl;
            codeblock << INDENT << "DIV CX" << endl;
            break;
        case '%' :
            codeblock << INDENT << "XCHG CX, AX" << endl; 
            codeblock << INDENT << "CWD" << endl;
            codeblock << INDENT << "DIV CX" << endl;
            codeblock << INDENT << "MOV AX, DX" << endl; // Remainder in AX
            break;
        default:
            cout << "Something went wrong" << endl;
    }
}


string new_label(){
    global_label_count++;
    return "L"+to_string(global_label_count);
}


void generate_new_tf_labels(){
    tf_labels.push({new_label(), new_label()});
}

void place_true_label(){
    string true_label = tf_labels.top().first;
    codeblock << true_label << ':' << endl;
}