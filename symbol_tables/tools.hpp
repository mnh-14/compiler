#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <iostream>
#include <string>

#define LOG(msg) log(msg, __func__, __FILE__, __LINE__)
#define _LOG_DESTRUC false
#define FILE_STREAM true

void log(std::string mssg){
    std::cout << mssg << std::endl;
}

template <typename TYPE>
void log(TYPE mssg, const char* func, const char* file, int line){
    std::cerr << "[LOG]: " << mssg << "\tat " << file<< ": " << line << " in func: " << func << "()" << std::endl;
}


// void log(std::string mssg, const char* func, const char* file, int line){
//     std::cout << "[LOG]: " << mssg << "\tat " << file<< ": " << line << " in func: " << func << "()" << std::endl;
// }

// void log(int mssg, const char* func, const char* file, int line){
//     std::cout << "[LOG]: " << mssg << "\tat " << file<< ": " << line << " in func: " << func << "()" << std::endl;
// }


#endif