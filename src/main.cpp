#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "parser/treeprinter.hpp"
#include <vector>
#include <exception>

using namespace std;

int main(){
    //TODO: buat while loop
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
    
    // Milestone 1
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

    // Milestone 2
    ofstream output2("test/milestone-2/output.txt");
    //TODO: hilangkan try catch
    try{
        Parser parser(parserTokens);
        std::unique_ptr<ParseNode> root = parser.program();
        cout << printTree(root.get(), "", true);
        output2 << printTree(root.get(), "", true);
        output2.close();
    }
    catch (std::exception& e){
        std::cout << e.what() << std::endl;
    }
    //TODO: tampilkan error message jika parser tidak valid

    // Milestone 3
}