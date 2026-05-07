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