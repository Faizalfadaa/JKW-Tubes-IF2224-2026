#include "lexer/lexer.hpp"
#include <vector>

using namespace std;

int main(){
    Reader reader;
    string path;

    //Input file
    do {
        cout << "Input path file: ";
        cin >> path;
    } while (!reader.open(path));

    //Proses setiap karakter
    Lexer lex(reader);
    vector<Token> tokens;
    while (!reader.isEOF()){
        Token token = lex.getNextToken();
        tokens.push_back(token);
    }
 
    ofstream output("test/milestone-1/output.txt");
    vector<Token> parserTokens; 
    for (const Token& token : tokens){
        output << token.str_type();
        if (token.lexeme != "") output << " (" << token.lexeme << ")";
        output << endl;
 
        if (token.type == TokenType::COMMENT)       continue;
        if (token.type == TokenType::NOTDETERMINED) continue;
        parserTokens.push_back(token);
    }
    output.close();

}