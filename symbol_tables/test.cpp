#include "./string_splitter.hpp"
#include "./symbol_info.hpp"
#include "./hash_functions.hpp"
#include "./scope_table.hpp"
#include "./tools.hpp"
#include <string>
#include <iostream>

using namespace std;

void test1(){
    string str;

    cout << "Enter the string that you want to split: ";
    std::getline(std::cin, str);

    string *splitted;
    int size = split_string(str, splitted, true);
    for(int i=0; ; i++){
        if (splitted[i]=="\n")
            break;
        cout << splitted[i] << endl;
    }
    cout << endl << "'Try again:'"<< endl;
    for (int i=0; i<size; i++){
        cout << splitted[i] << endl;
    }
    
    cout << "Number of words " << size;
}

void test2(){
    string* args = new string[5]{"INT", "INT", "STRING", "FLOAT", "\n"};
    SymbolInfo* si = new FunctionSymbolInfo("foo", "FUNCTION", "INT", args+1);
    cout << si->get_string_repr() << endl;
}

void test3(){
    // string * symbols = new string[5]{"abc", "INT", "123", "NUMBER", "\n"};
    string * symbols = new string[9]{"abc", "INT", "123", "NUMBER", "hello", "STRING", "pqr", "FLOAT", "\n"};
    // string * symbols = new string[1]{"\n"};
    cout << "here ??????????????" << endl;
    SymbolInfo* si = new StructSymbolInfo("myStruc", "STRUC", symbols);
    cout << si->get_string_repr() << endl;
}

void test4(){
    string s;
    while(true){
        cout << "Enter the string of your choice: ";
        getline(cin, s);
        cout << sdbm_hash(s, 7) << " this is the hash of your chosen " << endl << endl;
    }
}

void test5(){
    std::string* args = new std::string[2]{"INT", "\n"};
    
    string* symbols = new string[5]{"real", "INT", "imaginaty", "INT", "\n"};
    ScopeTable st(8, 1);
    st.set_hash_function(sdbm_hash);
    SymbolInfo *si1 = new SymbolInfo("123", "NUMBER");
    SymbolInfo *si2 = new SymbolInfo("A", "CHAR");
    SymbolInfo *si3 = new FunctionSymbolInfo("fibo", "FUMCTION", "INT", args);
    SymbolInfo *si4 = new StructSymbolInfo("complex_num", "STRUC", symbols);
    int * pos = new int[2];
    st.insert_symbol(si1, pos);
    LOG(pos[0]);
    st.insert_symbol(si2, pos);
    LOG(pos[0]);
    st.insert_symbol(si3, pos);
    LOG(pos[0]);
    bool stat = st.insert_symbol(si3, pos);
    LOG(pos[0]);
    st.insert_symbol(si4, pos);
    LOG(pos[0]);
    st.delete_symbol("A");
    cout << st.printable_scope_string() << endl;
}

void test6(){
    string* symbols = new string[7]{"INT", "n_doors", "BOOL", "is_electric", "STRING", "brand", "\n"};
    ScopeTable st(7, 2, sdbm_hash);
    SymbolInfo *si1 = new SymbolInfo("23", "NUMBER");
    SymbolInfo *si2 = new SymbolInfo("brand", "VAR");
    SymbolInfo *si3 = new StructSymbolInfo("car", "STRUC", symbols);
    int * pos = new int[2];
    st.insert_symbol(si1, pos);
    LOG("Inserted at position " + to_string(pos[0]) + " ," + to_string(pos[1]));
    st.insert_symbol(si2, pos);
    LOG("Inserted at position " + to_string(pos[0]) + " ," + to_string(pos[1]));
    st.insert_symbol(si3, pos);
    LOG("Inserted at position " + to_string(pos[0]) + " ," + to_string(pos[1]));
    cout << st.printable_scope_string() << endl;
    st.delete_symbol("brand");
    cout << "After delete" << endl;
    cout << st.printable_scope_string() << endl;
    
}

int main(){
    test6();
}