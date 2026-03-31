#include "lexer.hpp"

using namespace std;

Token Lexer::process(char c){
    //Untuk mempermudah pengecekan angka dan huruf
    bool isNumber = isdigit(c), isLetter = isalpha(c);

    switch (state){
        case (State::START):
            if(true){ 

            }else if(false){

            }else{

            }

    }

    //TODO: proses character berdasarkan DFA
}