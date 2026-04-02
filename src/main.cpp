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

    ofstream output("test/milestone-1/output.txt");
    //Proses setiap karakter
    Lexer lex(reader);
    while (!reader.isEOF()){
        Token token = lex.getNextToken();
        output << token.str_type();
        if (token.lexeme != "") output << " (" << token.lexeme << ")";
        output << endl;
    }
    output.close();
}