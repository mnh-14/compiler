#ifndef STRING_SPLITER_HPP
#define STRING_SPLITER_HPP


#include <string>
#include <cstring>
#include "./tools.hpp"

// int split_string(std::string str, std::string*& copy_to, int& size){
//     int strlen = str.length();
//     char * characters = new char[strlen+1];
//     strcpy(characters, str.c_str());
//     int word_count = 0;
//     int word_index[strlen];
//     word_index[0] = 0;
    
//     for(int i=0; i<strlen; i++){
//         if(characters[i] == ' '){
//             word_count++;
//             word_index[word_count] = i+1;
//             characters[i] == '\0';
//         }
//         else if(characters[i] == '\n'){
//             break;
//         }
//     }
//     word_count++;
//     // delete[] copy_to; // in case memory space for this whas declared
//     copy_to = new std::string[word_count];
//     char *word;
//     for (int i=0; i<word_count-1; i++){
//         word = (characters + word_index[i]);        
//         copy_to[i] = std::string(word);
//     }
//     copy_to[word_count-1] = std::string("\n");
//     size=word_count;

//     return word_count;
// }


int split_string(std::string str, std::string*& copy_to){
    int strlen = str.length();
    std::string* words = new std::string[strlen];
    int word_count = 0, word_start = -1, word_size;

    for (int i=0; i<strlen; i++){
        if(str.at(i) != ' ' && str.at(i) != '\n' && word_start < 0){ // the start of a new word
            word_start = i;
        }
        else if(str.at(i) == ' ' && word_start >= 0){ // new word had started, and now the new word has ended
            word_size = i - word_start;
            words[word_count] = str.substr(word_start, word_size);
            word_count++;
            word_start = -1; // a word ended
        }
        if(str.at(i)=='\n' || str.at(i)=='\0'){
            strlen = i;
            break;
        }
    }
    if(word_start >= 0){ // a new word started, but the sentence ended before finishing the final word
        words[word_count] = str.substr(word_start, strlen-word_start);
        word_count++;
    }
    
    // free copy_to, if needed
    copy_to = new std::string[word_count+1];
    for(int i=0; i<word_count; i++){
        copy_to[i] = words[i];        
    }
    copy_to[word_count] = "\n";
    delete[] words;
    return word_count;
}

int split_string(std::string str, std::string*& copy_to, bool space_optimized){
    if (space_optimized==true){
        return split_string(str, copy_to);
    }
    int strlen = str.length();
    std::string* words = new std::string[strlen];
    int word_count = 0, prev_word_idx = 0, word_size;

    for (int i=0; i<strlen; i++){
        if(str.at(i) == ' '){
            word_size = i - prev_word_idx;
            words[word_count] = str.substr(prev_word_idx, word_size);
            word_count++;
            prev_word_idx = i+1;
        }
        // if(str.at(i)=='\n' || str.at(i)=='/0')
        //     break;
    }
    words[word_count] = str.substr(prev_word_idx, strlen-prev_word_idx);
    word_count++;
    
    // free copy_to, if needed
    copy_to = new std::string[word_count+1];
    for(int i=0; i<word_count; i++){
        copy_to[i] = words[i];
    }
    copy_to[word_count] = "\n";
    return word_count;
}






#endif