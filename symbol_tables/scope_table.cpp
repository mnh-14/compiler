
#include <iostream>
#include <string>
#include "symbol_info.hpp"
#include "./tools.hpp"
#include "./scope_table.hpp"


ScopeTable::ScopeTable(int bucket_size, int scope_no, unsigned int (*func)(std::string, unsigned int)){
    this->bucket_size = bucket_size;
    this->scope_no = scope_no;
    this->hash_function = func;
    this->symbol_infos = new SymbolInfo*[bucket_size]();
}


ScopeTable::ScopeTable(int bucket_size, int scope_no){
    this->bucket_size = bucket_size;
    this->scope_no = scope_no;
    this->symbol_infos = new SymbolInfo*[bucket_size]();
}


void ScopeTable::set_hash_function(unsigned int (*func)(std::string, unsigned int)){
    this->hash_function = func;
}


void ScopeTable::set_parent_scope(ScopeTable* parent){
    this->parent_scope = parent;
    this->parent_scope->sub_scope_count++;
    this->label = this->parent_scope->label + "." + std::to_string(this->parent_scope->sub_scope_count);
}


ScopeTable* ScopeTable::get_parent_scope(){ return this->parent_scope; }


int ScopeTable::get_scope_no(){ return this->scope_no; }


std::string ScopeTable::get_label(){ return this->label; }


std::string ScopeTable::printable_scope_string(){ return printable_scope_string("\t"); }


bool ScopeTable::insert_symbol(SymbolInfo* s_info, int * insertion_pos){
    unsigned int idx = this->hash_function(s_info->get_symbol(), this->bucket_size);

    if(this->symbol_infos[idx] == nullptr){
        this->symbol_infos[idx] = s_info;
        if(insertion_pos!=nullptr){ insertion_pos[0] = idx+1; insertion_pos[1] = 1; }
        return true;
    }
    SymbolInfo * temp = this->symbol_infos[idx];
    int pos_count = 2;
    while(temp->get_next_symbolinfo() != nullptr){
        // LOG("TEMP INFO: "+temp->get_symbol()+"-"+temp->get_symbol_type());
        // LOG("Inserting: "+s_info->get_symbol()+"-"+s_info->get_symbol_type());
        // LOG(temp->get_symbol() == s_info->get_symbol());
        if(temp->get_symbol() == s_info->get_symbol())
            return false;
        temp = temp->get_next_symbolinfo();
        pos_count++;
    }
    if(temp->get_symbol() == s_info->get_symbol())
        return false;
    temp->set_next_symbolinfo(s_info);
    if(insertion_pos!=nullptr){ insertion_pos[0] = idx+1; insertion_pos[1] =  pos_count; }
    return true;
}

// bool ScopeTable::insert_symbol(SymbolInfo* s_info){
//     return this->insert_symbol(s_info, nullptr);
// }

SymbolInfo* ScopeTable::lookup(std::string symbol, int* pos){
    int idx = this->hash_function(symbol, this->bucket_size);
    SymbolInfo* temp=this->symbol_infos[idx];
    for(int i=1; temp!=nullptr; temp=temp->get_next_symbolinfo(), i++){
        if (temp->get_symbol() == symbol){
            if(pos != nullptr){ pos[0] = idx+1; pos[1] = i; }
            return temp;
        }
    }
    return nullptr;
}


// SymbolInfo* ScopeTable::lookup(std::string symbol){
//     return this->lookup(symbol, nullptr);
// }

bool ScopeTable::delete_symbol(std::string symbol, int* pos){
    int idx = this->hash_function(symbol, this->bucket_size);
    if(symbol_infos[idx]==nullptr) return false;
    SymbolInfo* temp = symbol_infos[idx];
    if(temp->get_symbol()==symbol){
        symbol_infos[idx] = temp->get_next_symbolinfo();
        delete temp;
        if(pos != nullptr) pos[0]=idx+1; pos[1]=1;
        return true;
    }
    SymbolInfo* desired; int pos_count=1;
    while(temp->get_next_symbolinfo()!=nullptr){
        desired = temp->get_next_symbolinfo();
        // LOG(desired->get_symbol());
        pos_count++;
        if(desired->get_symbol() == symbol){
            temp->set_next_symbolinfo(desired->get_next_symbolinfo());
            delete desired;
            if(pos != nullptr) pos[0]=idx+1; pos[1]=pos_count;
            return true;
        }
        temp = temp->get_next_symbolinfo();
    }
    return false;
}

std::string ScopeTable::printable_scope_string(std::string indent){
    std::string represent = "";
    represent = represent + indent + "ScopeTable # " + this->label;
    SymbolInfo* curr;
    
    for(int i=0; i<bucket_size; i++){
        curr = this->symbol_infos[i];
        if(curr==nullptr) continue;
        represent = represent + "\n" + indent + std::to_string(i) + " --> ";
        // LOG(i);
        while(curr != nullptr){
            represent = represent + curr->get_string_repr();
            curr = curr->get_next_symbolinfo();
        }
        // represent += "\n";
    }
    return represent;
}

ScopeTable::~ScopeTable(){
    if (symbol_infos==nullptr) return;
    SymbolInfo *curr, *prev;
    for(int i=0; i<bucket_size; i++){
        curr = this->symbol_infos[i];
        while(curr != nullptr){
            prev = curr;
            curr = curr->get_next_symbolinfo();
            delete prev;
        }
    }
    delete[] symbol_infos;
    if(_LOG_DESTRUC)
        std::cout << "\tScopeTable# " << this->scope_no << " removed" << std::endl;
}

