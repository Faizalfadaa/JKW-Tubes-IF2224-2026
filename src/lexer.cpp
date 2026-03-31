#include "lexer.hpp"

using namespace std;

void Lexer::process(char c){
    //Untuk mempermudah pengecekan angka dan huruf
    bool isNumber = isdigit(c), isLetter = isalpha(c);

    //TODO: proses character berdasarkan DFA
}