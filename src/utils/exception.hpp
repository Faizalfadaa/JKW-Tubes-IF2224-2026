#include <exception>
#include <vector>
#include <string>
#include "../lexer/token.hpp"

class ParsingError : public std::exception {
private:
    std::vector<TokenType> expectedTokens;
    TokenType foundToken;
    std::string message;

public:
    ParsingError(TokenType expectedToken, TokenType found){
        expectedTokens.push_back(expectedToken);
        foundToken = found;
        message = "unexpected token " + Token::toString(foundToken) + ", expected ";
        message += Token::toString(expectedToken);
    }

    ParsingError(std::vector<TokenType> expectedTokens, TokenType found){
        for (TokenType token : expectedTokens){
            this->expectedTokens.push_back(token);
        }
        foundToken = found;
        for (size_t i = 0; i < expectedTokens.size(); i++){
            message += Token::toString(expectedTokens.at(i));
            if (i != expectedTokens.size()-1){
                message += ",";
            }
        }
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class IdentRedeclarationError : public std::exception {
private:
    std::string identName;
    std::string message;

public:
    IdentRedeclarationError(const std::string& identName){
        this->identName = identName;
        message = identName + " already declared.";
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class SymbolTableError : public std::exception {
private:
    std::string message;

public:
    SymbolTableError(const std::string& msg){
        message = "Symbol table error: " + msg;
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class SemanticError : public std::exception {
private:
    std::string message;

public:
    SemanticError(const std::string& msg){
        message = "Semantic Error: " + msg;
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};