#include "lexer.hpp"

using namespace std;

Token Lexer::process(char c){
    //Untuk mempermudah pengecekan angka dan huruf
    bool isNumber = isdigit(c), isLetter = isalpha(c);

    //Proses karakter sesuai dengan state saat ini
    switch (state){
        case (State::START):
            if(isNumber){
                state = State::INTCON;
            } else if(c == 'a') {
                state = State::KW_A;
            }
            } else {
                state = State::UNKNOWN;
            }
            break;

        case (State::INTCON):
            if(c == '.'){
                state = State::INTDOT;
            }else if (isLetter){
                state = State::UNKNOWN;
            } else {
                
            }
            break;

        case (State::INTDOT):
            if(isNumber){
                state = State::REALCON;
            } else {
                state = State::
            }
            break;

        case (State::REALCON):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::OPENQUOTE):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::CHARCON):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::STRING):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::INQUOTE):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::CLOSEQUOTE):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::EQUALSIGN):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::EQL):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::LSS):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::LEQ):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::NEQ):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::GTR):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::GEQ):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::COLON):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::BECOMES):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::PLUS):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::MINUS):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::TIMES):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::RDIV):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::LPARENT):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::RPARENT):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::LBRACK):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::RBRACK):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::COMMA):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::SEMICOLON):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::PERIOD):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_A):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_AN):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::ANDSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_AR):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_ARR):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_ARRA):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::ARRAYSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_B):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_BE):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_BEG):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_BEGI):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::BEGINSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_C):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_CA):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_CAS):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::CASESY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_CO):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_CON):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_CONS):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::CONSTSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_D):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_DI):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::IDIV):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::DOSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_DOW):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_DOWN):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_DOWNT):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::DOWNTOSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_E):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_EL):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_ELS):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;
            state = State::
        case (State::ELSESY):
            if(true){   state = State::
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_EN):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::ENDSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_F):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_FO):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::FORSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_FU):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_FUN):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_FUNC):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_FUNCT):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_FUNCTI):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_FUNCTIO):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::FUNCTIONSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_I):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::IFSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_M):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_MO):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::IMOD):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_N):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_NO):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::NOTSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_O):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::OFSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::ORSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_P):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_PR):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_PRO):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_PROC):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_PROCE):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_PROCED):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_PROCEDU):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_PROCEDUR):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::PROCEDURESY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_PROG):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_PROGR):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_PROGRA):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::PROGRAMSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_R):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_RE):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_REC):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_RECO):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_RECOR):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::RECORDSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_REP):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_REPE):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_REPEA):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::REPEATSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_T):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_TH):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_THE):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::THENSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::TOSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_TY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_TYP):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::TYPESY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_U):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_UN):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_UNT):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_UNTI):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::UNTILSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_V):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_VA):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::VARSY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_W):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_WH):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_WHI):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::KW_WHIL):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::WHILESY):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::IDENT):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::OPENCUR):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::COMMENT):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

        case (State::CLOSECUR):
            if(true){
                state = State::
            } else {
                state = State::
            }
            break;

    }

    //TODO: proses character berdasarkan DFA
}