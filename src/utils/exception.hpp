#include <exception>
#include <vector>
#include <string>
#include "token.hpp"

class ParsingError : public std::exception {
private:
    std::vector<TokenType> expectedTokens;
    TokenType foundToken;

public:
    ParsingError(TokenType expectedToken, TokenType found){
        expectedTokens.push_back(expectedToken);
        foundToken = found;
    }

    ParsingError(std::vector<TokenType> expectedTokens, TokenType found){
        for (TokenType token : expectedTokens){
            this->expectedTokens.push_back(token);
        }
        foundToken = found;
    }

    const char* what() const noexcept override {
        std::string output = "Syntax error: unexpected token " + Token::toString(foundToken) + ", expected ";
        for (int i = 0; i < expectedTokens.size(); i++){
            output += Token::toString(expectedTokens.at(i));
            if (i != expectedTokens.size()-1){
                output += ",";
            }
        }
        return output.c_str();
    }
};