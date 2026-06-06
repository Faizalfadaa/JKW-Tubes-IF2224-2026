#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "parser/treeprinter.hpp"
#include "semantic/asttree.hpp"
#include "semantic/astvisitor.hpp"
#include "semantic/astprinter.hpp"
#include "interpreter/irgenerator.hpp"
#include "interpreter/stackinterpreter.hpp"
#include <vector>
#include <exception>

using namespace std;

int main(){
    while (true){
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
        try{
            Parser parser(parserTokens);
            std::unique_ptr<ParseNode> root = parser.program();
            cout << printTree(root.get(), "", true);
            output2 << printTree(root.get(), "", true);
            output2.close();
            if (parser.hasErrorNode){
                std::cout << "=================================================================================" << std::endl <<
                             "Terdapat struktur yang tidak valid. Tidak bisa lanjut ke tahap Semantic Analysis." << std::endl <<
                             "=================================================================================" << std::endl << std::endl;
                continue;
            }


        // Milestone 3
            ASTBuilder astBuilder;
            unique_ptr<ASTNode> astRoot = astBuilder.createAST(root.get());

            ASTVisitor visitor = ASTVisitor();
            astRoot->accept(&visitor);

            string astTreeOutput = printDecoratedAST(astRoot.get(), "", true);
            string symbolTableOutput = visitor.getSymbolTable().printAll();

            cout << "\n===== DECORATED AST =====\n";
            cout << astTreeOutput;

            cout << "\n===== SYMBOL TABLE =====\n";
            cout << symbolTableOutput;

            ofstream output3("test/milestone-3/output.txt");
            output3 << "===== DECORATED AST =====\n";
            output3 << astTreeOutput;
            output3 << "\n===== SYMBOL TABLE =====\n";
            output3 << symbolTableOutput;
            output3.close();
            

        // Milestone 4
            IntermediateCodeGenerator generator;
            std::vector<Instruction> code = generator.generate(astRoot.get());
            StackInterpreter interpreter(code);
            std::string programOutput = interpreter.run();

            std::ostringstream result;
            result << "===== INSTRUCTION =====\n";
            result << instructionsToString(code);
            result << "\n===== PROGRAM OUTPUT =====\n";
            result << programOutput;

            ofstream output4("test/milestone-4/output.txt");
            output4 << result.str();
            output4.close();
        }
        catch (std::exception& e){
            std::cout << e.what() << std::endl;
            continue;
        }
    }
}