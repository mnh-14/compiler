#include <iostream>
#include "./symbol_info.hpp"
#include "./scope_table.hpp"
#include "./symbol_table.hpp"
#include "./tools.hpp"
#include "./string_splitter.hpp"
#include "./hash_functions.hpp"


using namespace std;

const string TAB = "\t";
void show_command(string* cmd_parts, int cmd_no);
void handle_incesrtion(SymbolTable& symbol_table, string* cmd_parts, int part_count);
void handle_lookup(SymbolTable& symbol_table, string* cmd_parts, int part_count);
void handle_deletion(SymbolTable& symbol_table, string* cmd_parts, int part_count);
void handle_printing(SymbolTable& symbol_table, string* cmd_parts, int part_count);
void handle_entering_scope(SymbolTable& symbol_table, string* cmd_parts, int part_count);
void handle_exiting_scope(SymbolTable& symbol_table, string* cmd_parts, int part_count);
void parameter_missmatch_print(string cmd, string indent=TAB);


int main(){
    int bucket;
    // cout << "Enter Bucket size: ";
    cin >> bucket;
    cin.ignore();
    SymbolTable symbol_table(bucket, sdbm_hash);
    cout << "\tScopeTable# 1 created" << endl;
    
    string cmd, *cmd_parts;
    int cmd_len, cmd_count=0;
    bool running = true;
    while(running){
        getline(cin, cmd);
        cmd_len = split_string(cmd, cmd_parts);
        cmd_count++;
        if(cmd_len < 0) continue;
        
        show_command(cmd_parts, cmd_count);

        switch (cmd_parts[0][0]) {
        case 'I':
            handle_incesrtion(symbol_table, cmd_parts, cmd_len);
            break;
        case 'L':
            handle_lookup(symbol_table, cmd_parts, cmd_len);
            break;
        case 'D':
            handle_deletion(symbol_table, cmd_parts, cmd_len);
            break;
        case 'P':
            handle_printing(symbol_table, cmd_parts, cmd_len);
            break;
        case 'S':
            handle_entering_scope(symbol_table, cmd_parts, cmd_len);
            break;
        case 'E':
            handle_exiting_scope(symbol_table, cmd_parts, cmd_len);
            break;
        case 'Q':
            running = false;
            break;
        default:
            break;
        }

        
    }

    
}


void show_command(string* cmd_parts, int cmd_no){
    cout << "Cmd " << cmd_no << ":";
    for(int i=0; cmd_parts[i]!="\n"; i++){
        cout << " " << cmd_parts[i];
    }
    cout << endl;
}

void parameter_missmatch_print(string cmd, string indent){
    cout << indent << "Number of parameters mismatch for the command " << cmd << endl;
}


void handle_incesrtion(SymbolTable& symbol_table, string* cmd_parts, int part_count){
    if(part_count < 3)
        return parameter_missmatch_print("I");
    if(cmd_parts[2]=="FUNCTION" && part_count<4)
        return parameter_missmatch_print("I");
    if((cmd_parts[2]=="STRUCT" || cmd_parts[2]=="UNION") && (part_count-3)&1 == 1)
        return parameter_missmatch_print("I");
    int* location = new int[3];
    bool success = symbol_table.insert_symbol(cmd_parts+1, location);
    if(success)
        cout << TAB << "Inserted in ScopeTable# " << location[0] << " at position " << location[1] << ", " << location[2] << endl;
    else
        cout << TAB << "'" << cmd_parts[1] << "' alreay exists in the current ScopeTable" << endl;
    
}

void handle_lookup(SymbolTable& symbol_table, string* cmd_parts, int part_count){
    if(part_count != 2)
        return parameter_missmatch_print(cmd_parts[0]);
    int * location = new int[3];
    SymbolInfo* si = symbol_table.lookup(cmd_parts[1], location);
    if (si != nullptr)
        cout << TAB << "'" << cmd_parts[1] << "' found in ScopeTable# " << location[0] << " at position " << location[1] << ", " << location[2] << endl;
    else 
        cout << TAB << "'" << cmd_parts[1] << "' not found in any of the ScopeTables" << endl;
}

void handle_deletion(SymbolTable& symbol_table, string* cmd_parts, int part_count){
    // Deleted '==' from ScopeTable# 5 at position 2, 1
    // Not found in the current ScopeTable
    if (part_count != 2)
        return parameter_missmatch_print(cmd_parts[0]);
    
    int * loc = new int[3];
    bool success = symbol_table.remove_symbol(cmd_parts[1], loc);
    if(success)
        cout << TAB << "Deleted '" << cmd_parts[1] << "' from ScopeTable# " << loc[0] << " at position " << loc[1] << ", " << loc[2] << endl;
    else
        cout << TAB << "Not found in the current ScopeTable" << endl;
}


void handle_entering_scope(SymbolTable& symbol_table, string* cmd_parts, int part_count){
    // ScopeTable# 3 created
    if(part_count != 1)
        return parameter_missmatch_print(cmd_parts[0]);
    
    int scope_no = symbol_table.enter_new_scope();
    cout << TAB << "ScopeTable# " << scope_no << " created" << endl;
}


void handle_exiting_scope(SymbolTable& symbol_table, string* cmd_parts, int part_count){
    if(part_count != 1)
        return parameter_missmatch_print(cmd_parts[0]);
    
        symbol_table.exit_scope();
}

void handle_printing(SymbolTable& symbol_table, string* cmd_parts, int part_count){
    if(part_count != 2)
        return parameter_missmatch_print(cmd_parts[0]);
    if (cmd_parts[1]=="C")
        cout << symbol_table.current_scope_string() << endl;
    else if (cmd_parts[1]=="A")
        cout << symbol_table.all_scope_string();
    else 
        cout << TAB << "Parameter mismatch for the command " << cmd_parts[1] << endl;
    
}