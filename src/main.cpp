#include "lexer/lexer.hpp"

int main(){
    Reader reader;
    std::string path;

    //Input file
    do {
        std::cout << "Input path file: ";
        std::cin >> path;
    } while (!reader.open(path));

    //Proses setiap karakter
    Lexer lex(reader);
    while (!reader.isEOF()){
        lex.getNextToken();
    }
}