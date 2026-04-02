#include "lexer/lexer.hpp"

using namespace std;

int main(){
    Reader reader;
    string path;

    //Input file
    do {
        cout << "Input path file: ";
        cin >> path;
    } while (!reader.open(path));

    ofstream output("output.txt");
    //Proses setiap karakter
    Lexer lex(reader);
    while (!reader.isEOF()){
        Token token = lex.getNextToken();
        output << token.str_type() + "(" << token.lexeme << ")" << endl;
    }
    output.close();
}