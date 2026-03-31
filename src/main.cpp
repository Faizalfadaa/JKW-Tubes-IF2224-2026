#include "lexer.hpp"

int main(){
    Reader r;
    std::string path;

    //Input file
    do {
        std::cout << "Input path file: ";
        std::cin >> path;
    } while (!r.open(path));

    //Proses setiap karakter
    Lexer lex;
    while (!r.isEOF()){
        // std::cout << r.get(); //process every char here
        lex.process(r.get());
        r.next();
    }
}