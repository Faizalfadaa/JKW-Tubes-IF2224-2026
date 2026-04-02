//List token
enum class TokenType{
    INTCON,
    REALCON,
    CHARCON,
    STRING,
    NOTSY,
    PLUS,
    MINUS,
    TIMES,
    IDIV,
    RDIV,
    IMOD,
    ANDSY,
    ORSY,
    EQL,
    NEQ,
    GTR,
    GEQ,
    LSS,
    LEQ,
    LPARENT,
    RPARENT,
    LBRACK,
    RBRACK,
    COMMA,
    SEMICOLON,
    PERIOD,
    COLON,
    BECOMES,
    CONSTSY,
    TYPESY,
    VARSY,
    FUNCTIONSY,
    PROCEDURESY,
    ARRAYSY,
    RECORDSY,
    PROGRAMSY,
    IDENT,
    BEGINSY,
    IFSY,
    CASESY,
    REPEATSY,
    WHILESY,
    FORSY,
    ENDSY,
    ELSESY,
    UNTILSY,
    OFSY,
    DOSY,
    TOSY,
    DOWNTOSY,
    THENSY,
    COMMENT,
    UNKNOWN,
    NOTDETERMINED
};

class Token{
public:
    TokenType type;
    std::string lexeme;

    //Konstruktor
    Token(TokenType type, std::string lexeme) : type(type), lexeme(lexeme) {}
    std::string str_type(){  
        switch (type){
            case TokenType::INTCON:
                return "INTCON";
            case TokenType::REALCON:
                return "REALCON";
            case TokenType::CHARCON:
                return "CHARCON";
            case TokenType::STRING:
                return "STRING";
            case TokenType::NOTSY:
                return "NOTSY";
            case TokenType::PLUS:
                return "PLUS";
            case TokenType::MINUS:
                return "MINUS";
            case TokenType::TIMES:
                return "TIMES";
            case TokenType::IDIV:
                return "IDIV";
            case TokenType::RDIV:
                return "RDIV";
            case TokenType::IMOD:
                return "IMOD";
            case TokenType::ANDSY:
                return "ANDSY";
            case TokenType::ORSY:
                return "ORSY";
            case TokenType::EQL:
                return "EQL";
            case TokenType::NEQ:
                return "NEQ";
            case TokenType::GTR:
                return "GTR";
            case TokenType::GEQ:
                return "GEQ";
            case TokenType::LSS:
                return "LSS";
            case TokenType::LEQ:
                return "LEQ";
            case TokenType::LPARENT:
                return "LPARENT";
            case TokenType::RPARENT:
                return "RPARENT";
            case TokenType::LBRACK:
                return "LBRACK";
            case TokenType::RBRACK:
                return "RBRACK";
            case TokenType::COMMA:
                return "COMMA";
            case TokenType::SEMICOLON:
                return "SEMICOLON";
            case TokenType::PERIOD:
                return "PERIOD";
            case TokenType::COLON:
                return "COLON";
            case TokenType::BECOMES:
                return "BECOMES";
            case TokenType::CONSTSY:
                return "CONSTSY";
            case TokenType::TYPESY:
                return "TYPESY";
            case TokenType::VARSY:
                return "VARSY";
            case TokenType::FUNCTIONSY:
                return "FUNCTIONSY";
            case TokenType::PROCEDURESY:
                return "PROCEDURESY";
            case TokenType::ARRAYSY:
                return "ARRAYSY";
            case TokenType::RECORDSY:
                return "RECORDSY";
            case TokenType::PROGRAMSY:
                return "PROGRAMSY";
            case TokenType::IDENT:
                return "IDENT";
            case TokenType::BEGINSY:
                return "BEGINSY";
            case TokenType::IFSY:
                return "IFSY";
            case TokenType::CASESY:
                return "CASESY";
            case TokenType::REPEATSY:
                return "REPEATSY";
            case TokenType::WHILESY:
                return "WHILESY";
            case TokenType::FORSY:
                return "FORSY";
            case TokenType::ENDSY:
                return "ENDSY";
            case TokenType::ELSESY:
                return "ELSESY";
            case TokenType::UNTILSY:
                return "UNTILSY";
            case TokenType::OFSY:
                return "OFSY";
            case TokenType::DOSY:
                return "DOSY";
            case TokenType::TOSY:
                return "TOSY";
            case TokenType::DOWNTOSY:
                return "DOWNTOSY";
            case TokenType::THENSY:
                return "THENSY";
            case TokenType::COMMENT:
                return "COMMENT";
            case TokenType::UNKNOWN:
                return "UNKNOWN";
            case TokenType::NOTDETERMINED:
                return "";
        }
        return "Error";
    }
};
