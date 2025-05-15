%option c++
%option noyywrap

%{
    #include <iostream>
    #include <string>
    #include <fstream>

    using namespace std;
    string comment = "";
%}

VARIABLE [a-zA-Z_]+[a-zA-Z_0-9]*
LETTER [a-zA-Z]
%x COMMENT

%%

"/*" { BEGIN(COMMENT); comment=""; }
<COMMENT>"*/" { BEGIN(INITIAL); cout << "Found comment: " << comment << endl; }
<COMMENT>[^*]+ { comment += yytext; }
<COMMENT>"*" {comment += yytext; }

{LETTER}+\.{LETTER}+ { cout << "Got .... letter: " << yytext << endl; }
{LETTER}+ { cout << "Got Word: " << yytext << endl; }
. {/* ignore */}


%%

int main(int argc, char *argv[]){
    if(argc!=2){
        cerr << "Format <filename-to-scan>" << endl;
    }

	/* FILE *fin=fopen(argv[1],"r"); */
    ifstream inputfile(argv[1]);
    

    if(!inputfile.is_open()){
        cerr << "Could not open input file" << endl;
    }

    yyFlexLexer lexer(&inputfile);
    
    while(true){
        if(lexer.yylex() == 0)
            break;        
    }
    
    return 0;

}