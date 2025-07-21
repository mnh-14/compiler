#include "functions.hpp"

using namespace std;


Label::Label(string l): label(l) {}

static int global_label_count = 0;
static int loop_label_count = 0;
// stack<pair<string, string>> tf_labels;
vector<pair<Label*, Label*>> tf_labels;
stack<string> loop_label;
vector<pair<int, int>> tf_label_use_count;
vector<Label *> label_stream;
stack<pair<bool, bool>> tf_jumpable;
vector<MemLocation *> function_arg_mems;
int global_func_param_byte_size = 0;
FunctionSymbolInfo* current_func;
vector<pair<string, string>> function_parameters;


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
    codeblock << INDENT << "MOV AX, " << endl;
}


void insert_params_to_function(string vid) {
    // string parts[] = {vid, "VAR"};
    // auto si = symbol_table.insert_symbol(parts);
    // Type * t = Type::construct_type(global_type);
    // si->set_type(t);
    // global_func_param_byte_size+=t->size;
    // int off = symbol_table.get_space_for_local_variable(t);
    // auto mem = new MemLocation("bp", off);
    // function_arg_mems.push_back(mem);
    // si->set_memeory(mem);
    function_parameters.push_back({vid, global_type});
}

void finalize_func_params(string fid) {
    // for (auto &m: function_arg_mems){
    //     m->offset += (4+global_func_param_byte_size);
    // }
    // symbol_table.reset_stack_offset();
    // function_arg_mems.clear();
    // global_func_param_byte_size = 0;
    string parts[2];
    int off = 4;
    int total_parambytes = 0;
    string * params = new string[function_parameters.size()+1];
    int i = 0;
    for (auto p=function_parameters.rbegin(); p<function_parameters.rend(); ++p){
        parts[0] = (*p).first; parts[1] = "VAR";
        auto si = symbol_table.insert_symbol(parts);
        auto t = Type::construct_type((*p).second);
        si->set_memeory(new MemLocation("bp", off+total_parambytes));
        total_parambytes += t->get_size();
        si->set_type(t);
        params[i] = (*p).second;
        i++;
    }
    params[i] = "\n";
    current_func->set_arguments(params);
    current_func->set_arg_byte_size(total_parambytes);
    function_parameters.clear();
    symbol_table.reset_stack_offset();
}


void entering_function(string fid, string rettype, int line_no){
    // FunctionSymbolInfo* si;
    current_func = dynamic_cast<FunctionSymbolInfo*>(symbol_table.lookup(fid));
    if(!current_func){
        string parts[] = {fid, "FUNCTION", rettype, "\n"};
        current_func = dynamic_cast<FunctionSymbolInfo*>(symbol_table.insert_symbol(parts));
    }
    forced_enter_scope();
    codeblock << fid << " PROC" << INDENT << INDENT << "; " << line_no << " no. line" << endl;
    codeblock << INDENT << "PUSH BP" << endl;
    codeblock << INDENT << "MOV BP, SP" << endl;
}

void return_statement(){
    codeblock << INDENT << "MOV SP, BP" << endl;
    codeblock << INDENT << "POP BP" << endl;
    codeblock << INDENT << "RET " << current_func->get_arg_byte_size() << endl;
}

void finishing_function(string fid){
    if(current_func->get_return_type()=="void") return_statement();
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
        case '-' : codeblock << INDENT << "SUB CX, AX" << endl << INDENT << "MOV AX, CX" << endl; break;
    }
}

string get_memloc(string id){
    return symbol_table.lookup(id)->get_memory()->get_location();
}

void move(string a, string b, bool preserve, int line){
    if(preserve) push_asmcode(a);
    codeblock << INDENT << "MOV " << a << ", " << b << endl;
}

void mulop_asmcode(string mulop){
    switch(mulop[0]){
        case '*' :
            codeblock << INDENT << "CBW" << endl;
            codeblock << INDENT << "MUL BX" << endl;
            break;
        case '/' :
            codeblock << INDENT << "XCHG BX, AX" << endl; 
            codeblock << INDENT << "CWD" << endl;
            codeblock << INDENT << "DIV BX" << endl;
            break;
        case '%' :
            codeblock << INDENT << "XCHG BX, AX" << endl; 
            codeblock << INDENT << "CWD" << endl;
            codeblock << INDENT << "DIV BX" << endl;
            codeblock << INDENT << "MOV AX, DX" << endl; // Remainder in AX
            break;
        default:
            cout << "Something went wrong" << endl;
    }
}


Label* new_label(){
    global_label_count++;
    return new Label("L"+to_string(global_label_count));
}

string new_loop_label(){
    loop_label_count++;
    loop_label.push("LOOP_"+to_string(loop_label_count));
    return loop_label.top();
}


void generate_new_tf_labels(bool true_label, bool false_label){
    Label * tl, * fl;

    if(true_label) tl = new_label();
    else tl = tf_labels.back().first;

    if(false_label) fl = new_label();
    else fl = tf_labels.back().second;

    fl->ref_count++; tl->ref_count++;
    tf_labels.push_back({tl, fl});
    tf_label_use_count.push_back({0, 0});
}

void place_true_label(){ // Don't add the label if never used
    if(tf_labels.back().first->use_count==0) return;
    string true_label = tf_labels.back().first->label;
    codeblock << true_label << ':' << endl;
}

void place_false_label(){ // Don't add the label if never used
    if(tf_labels.back().second->use_count==0) return;
    string false_label = tf_labels.back().second->label;
    codeblock << false_label << ':' << endl;
}

void pop_used_tf_label(){
    auto p = tf_labels.back();
    tf_labels.pop_back();
    p.first->ref_count--; p.second->ref_count--;
    if(p.first->ref_count==0) delete p.first;
    if(p.second->ref_count==0) delete p.second;
}

void place_loop_label(){
    codeblock << new_loop_label() << ':' << endl;
}

void place_jump_loop(){
    codeblock << INDENT << "JMP " << loop_label.top() << endl;
}

void pop_loop_label(){
    loop_label.pop();
}


void write_assign_asmcode(string mem, bool is_simple){
    if(!is_simple){
        cout<<"Writing non simple expressions for assignment"<<endl;
        place_true_label();
        move("AX", "1");
        jump_to(get_new_single_label());
        place_false_label();
        move("AX", "0");
        place_single_label();
        pop_single_label();
    }
    move(mem, "AX");
}

void place_single_label(){
    auto l = label_stream.back();
    codeblock << l->label << ':' << endl;
}

void pop_single_label(){
    auto l = label_stream.back();
    label_stream.pop_back();
    delete l;
}

void jump_to_current_single_label(){
    jump_to(label_stream.back()->label);
    // if(tf_labels.back().first->use_count==0) cout << "HEllo";
}

string get_new_single_label(){
    auto l = new_label();
    label_stream.push_back(l);
    l->ref_count++;
    l->use_count++;
    return l->label;
}

void jump_to(string label){
    codeblock << INDENT << "JMP " << label << endl; 
}

string asmop_for_cmp(string cmpop){
    if(cmpop==">=") return "JGE";
    if(cmpop==">") return "JG";
    if(cmpop=="<=") return "JLE";
    if(cmpop=="<") return "JL";
    if(cmpop=="==") return "JE";
    if(cmpop=="!=") return "JNE";
    return "?";
}

string asmop_for_cmp(string cmpop, bool){
    if(cmpop==">=") return "JL";
    if(cmpop==">") return "JLE";
    if(cmpop=="<=") return "JG";
    if(cmpop=="<") return "JGE";
    if(cmpop=="==") return "JNE";
    if(cmpop=="!=") return "JE";
    return "?";
}

void jump_to_true(){
    codeblock << INDENT << "JMP " << tf_labels.back().first->label << endl;
    tf_labels.back().first->use_count++;
}
void jump_to_false(){
    codeblock << INDENT << "JMP " << tf_labels.back().second->label << endl;
    tf_labels.back().second->use_count++;
}

void compare_asmcode(string cmpop, bool pop_op1, string op1, string op2){
    codeblock << INDENT << "CMP " << op1 << ", " << op2 << endl;
    if(pop_op1) pop_asmcode(op1);
    if(tf_jumpable.empty()){
        codeblock << INDENT << asmop_for_cmp(cmpop, false) << " " << tf_labels.back().second->label << endl;
        tf_labels.back().second->use_count++;
    }
    else if(tf_jumpable.top().first){
        codeblock << INDENT << asmop_for_cmp(cmpop) << " " << tf_labels.back().first->label << endl;
        tf_labels.back().first->use_count++;
    }
    else if(tf_jumpable.top().second){
        codeblock << INDENT << asmop_for_cmp(cmpop, false) << " " << tf_labels.back().second->label << endl;
        tf_labels.back().second->use_count++;
    }
}

// void 
void simple_to_conditionals(bool is_simple){
    if(!is_simple) return;
    cout << "Turning a simple expressiont o conditionals in LOGICOP" << endl;
    codeblock << INDENT << "CMP AX, 0" << endl;
    if(tf_jumpable.empty()){
        codeblock << INDENT << "JE " << tf_labels.back().second->label << endl;
        tf_labels.back().second->use_count++;
        return;
    }
    if(tf_jumpable.top().first){
        codeblock << INDENT << "JNE " << tf_labels.back().first->label << endl;
        tf_labels.back().first->use_count++;
    }
    if(tf_jumpable.top().second){
        codeblock << INDENT << "JE " << tf_labels.back().second->label << endl;
        tf_labels.back().second->use_count++;
    }
}


void set_jumpable(bool tjump, bool fjump){
    tf_jumpable.push({tjump, fjump});
}


void log(string msg){
    cout << msg << endl;
}

void push_asmcode(string op){
    codeblock << INDENT << "PUSH " << op << endl;
}

void pop_asmcode(string op) {
    codeblock << INDENT << "POP " << op << endl;
}

void negate_operation(string addop, string operand){
    if (addop == "+") return;
    codeblock << INDENT << "NEG " << operand << endl;
}