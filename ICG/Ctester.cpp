#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include "antlr4-runtime.h"
#include "C8086Lexer.h"
#include "C8086Parser.h"
#include "../symbol_tables/symbol_table.hpp"

#define ASM_FILE "code.asm"
#define CODE_BLOCK "code.txt"
#define PRINTLN_LIBRARY "input/printable.txt"

#ifndef INDENT
#define INDENT "    "
#endif

using namespace antlr4;
using namespace std;

string global_type;

string outputDirectory; 

ofstream parserLogFile; // global output stream
ofstream errorFile; // global error stream
ofstream lexLogFile; // global lexer log stream

ofstream codeblock;
ofstream asmfile;
ifstream printlnfile;

SymbolTable symbol_table(7);
bool is_new_scoped = false;
vector<pair<string, string>> param_list;
vector<string> declared_ids;

string definitions[] = {"", "DB", "DW", "DD"};

int syntaxErrorCount;


int init_asm_code(){
    asmfile.open(outputDirectory+ASM_FILE);
    if (!asmfile.is_open()) {
        cerr << "Error opening asm file: " << ASM_FILE << endl;
        return 1;
    }
    
    asmfile << ".MODEL SMALL" << endl << ".STACK 1000H" << endl << ".DATA" << endl;
    asmfile << INDENT <<"number DB \"00000$\"" << endl;
    return 0;
}

int init_code_section(){
    codeblock.open(outputDirectory+CODE_BLOCK);
    if (!codeblock.is_open()) {
        cerr << "Error opening code block file: " << CODE_BLOCK << endl;
        return 1;
    }
    codeblock << ".CODE" << endl;
    codeblock << INDENT << "MOV AX, @DATA" << endl;
    codeblock << INDENT << "MOV DS, AX" << endl;
    codeblock << INDENT << "CALL main" << endl;
    codeblock << INDENT << "MOV AX, 4CH" << endl;
    codeblock << INDENT << "INT 21H" << endl;
    return 0;
}


int merge_asm_and_code_section(){
    codeblock.close();
    ifstream codestram(outputDirectory+CODE_BLOCK);
    if (!codestram.is_open()) {
        cerr << "Error opening code block file: " << CODE_BLOCK << endl;
        return 1;
    }

    asmfile << codestram.rdbuf();
    
    asmfile << endl;
    codestram.close();
    return 0;

}


int add_print_library(){
    printlnfile.open(PRINTLN_LIBRARY);
    if (!printlnfile.is_open()) {
        cerr << "Error opening code block file: " << PRINTLN_LIBRARY << endl;
        return 1;
    }
    
    asmfile<<endl;
    asmfile << printlnfile.rdbuf();

    printlnfile.close();
    asmfile << endl;
    return 0;
}

void close_all_files(){
    parserLogFile.close();
    errorFile.close();
    lexLogFile.close();
    asmfile.close();
}


int main(int argc, const char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    // ---- Input File ----
    ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        cerr << "Error opening input file: " << argv[1] << endl;
        return 1;
    }

    outputDirectory = "output/";
    string parserLogFileName = outputDirectory + "parserLog.txt";
    string errorFileName = outputDirectory + "errorLog.txt";
    string lexLogFileName = outputDirectory + "lexerLog.txt";

    // create output directory if it doesn't exist
    system(("mkdir -p " + outputDirectory).c_str());

    // ---- Output Files ----
    parserLogFile.open(parserLogFileName);
    if (!parserLogFile.is_open()) {
        cerr << "Error opening parser log file: " << parserLogFileName << endl;
        return 1;
    }

    errorFile.open(errorFileName);
    if (!errorFile.is_open()) {
        cerr << "Error opening error log file: " << errorFileName << endl;
        return 1;
    }

    lexLogFile.open(lexLogFileName);
    if (!lexLogFile.is_open()) {
        cerr << "Error opening lexer log file: " << lexLogFileName << endl;
        return 1;
    }

    if(init_asm_code()) return 1;

    if(init_code_section()) return 1;

    // ---- Parsing Flow ----
    ANTLRInputStream input(inputFile);
    C8086Lexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    C8086Parser parser(&tokens);

    // this is necessary to avoid the default error listener and use our custom error handling
    parser.removeErrorListeners();

    // start parsing at the 'start' rule
    parser.start();
    
    if(merge_asm_and_code_section()) return 1;
    if(add_print_library()) return 1;

    close_all_files();
    inputFile.close();

    cout << "Parsing completed. Check the output files for details." << endl;
    return 0;
}
