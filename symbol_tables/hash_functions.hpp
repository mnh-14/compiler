#ifndef HASH_FUNC_HPP
#define HASH_FUNC_HPP

#include<string>
#include<cstdint>
#include "./tools.hpp"


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

// SOURCE: http://www.cse.yorku.ca/~oz/hash.html
unsigned int djb2_hash(std::string str, unsigned int bucketsize){
    unsigned long int hash = 5381;
    for(int i=0; i<str.length(); i++){
        hash = (hash << 5) + hash + (unsigned char)str.at(i);
    }
    
    return (unsigned int)hash % bucketsize;
}

// Reference: http://www.isthe.com/chongo/tech/comp/fnv/
// And,     : https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
unsigned int fnv1a_hash(std::string str, unsigned int bucketsize){
    uint32_t hash = 2166136261u; // FNV offset basis for 32 bit;
    for(int i=0; i<str.length(); i++){
        hash = hash ^ (unsigned char)str.at(i);
        hash = hash * 16777619u; //FNV prime for 32 bit
    }
    return (unsigned int)hash % bucketsize;
}






#endif