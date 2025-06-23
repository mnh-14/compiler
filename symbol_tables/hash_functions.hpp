#ifndef HASH_FUNC_HPP
#define HASH_FUNC_HPP

#include<string>
#include<cstdint>
#include "./tools.hpp"


unsigned int sdbm_hash(std::string str, unsigned int num_buckets);

unsigned int sdbm_hash2(std::string str, unsigned int num_buckets);

unsigned int sdbm_hash(std::string str);

// SOURCE: http://www.cse.yorku.ca/~oz/hash.html
unsigned int djb2_hash(std::string str, unsigned int bucketsize);

// Reference: http://www.isthe.com/chongo/tech/comp/fnv/
// And,     : https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
unsigned int fnv1a_hash(std::string str, unsigned int bucketsize);



#endif