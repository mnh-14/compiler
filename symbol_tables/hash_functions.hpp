#ifndef HASH_FUNC_HPP
#define HASH_FUNC_HPP

#include<string>


unsigned int sdbm_hash(std::string str, unsigned int num_buckets){
    unsigned int hash = 0;
    unsigned int len = str.length();

    for(unsigned int i=0; i<len; i++){
        hash = ((str[i]) + (hash << 6) + (hash << 16) - hash) % num_buckets;
    }
    
    return hash;
}

unsigned int sdbm_hash(std::string str){
    return sdbm_hash(str, 53);
}





#endif