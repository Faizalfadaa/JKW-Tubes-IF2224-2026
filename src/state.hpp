enum class State{
    //Start State
    START,

    //Angka
    INTCON,
    INTDOT,
    REALCON,
    //End symbol = !angka

    //String dan Char
    OPENQUOTE,
    CHARCON,
    STRING,
    INQUOTE,
    CLOSEQUOTE,
    //End symbol = '

    //Biner Character Symbol
    EQUALSIGN,
    EQL,
    LSS,
    LEQ,
    NEQ,
    GTR,
    GEQ,
    COLON,
    BECOMES,

    //Single Character Symbol
    PLUS,
    MINUS,
    TIMES,
    RDIV,
    LPARENT,
    RPARENT,
    LBRACK,
    RBRACK,
    COMMA,
    SEMICOLON,
    PERIOD,

    //Huruf
    KW_A,
    KW_AN,
    ANDSY,
    KW_AR,
    KW_ARR,
    KW_ARRA,
    ARRAYSY,

    KW_B,
    KW_BE,
    KW_BEG,
    KW_BEGI,
    BEGINSY,
    
    KW_C,
    KW_CA,
    KW_CAS,
    CASESY,
    KW_CO,
    KW_CON,
    KW_CONS,
    CONSTSY,
    
    KW_D,
    KW_DI,
    IDIV,
    DOSY,
    KW_DOW,
    KW_DOWN,
    KW_DOWNT,
    DOWNTOSY,

    KW_E,
    KW_EL,
    KW_ELS,
    ELSESY,
    KW_EN,
    ENDSY,
    
    KW_F,
    KW_FO,
    FORSY,
    KW_FU,
    KW_FUN,
    KW_FUNC,
    KW_FUNCT,
    KW_FUNCTI,
    KW_FUNCTIO,
    FUNCTIONSY,
    
    KW_I,
    IFSY,
    
    KW_M,
    KW_MO,
    IMOD,

    KW_N,
    KW_NO,
    NOTSY,
    
    KW_O,
    OFSY,
    ORSY,
    
    KW_P,
    KW_PR,
    KW_PRO,
    KW_PROC,
    KW_PROCE,
    KW_PROCED,
    KW_PROCEDU,
    KW_PROCEDUR,
    PROCEDURESY,
    KW_PROG,
    KW_PROGR,
    KW_PROGRA,
    PROGRAMSY,
    
    KW_R,
    KW_RE,
    KW_REC,
    KW_RECO,
    KW_RECOR,
    RECORDSY,
    KW_REP,
    KW_REPE,
    KW_REPEA,
    REPEATSY,
    
    KW_T,
    KW_TH,
    KW_THE,
    THENSY,
    TOSY,
    KW_TY,
    KW_TYP,
    TYPESY,

    KW_U,
    KW_UN,
    KW_UNT,
    KW_UNTI,
    UNTILSY,
    
    KW_V,
    KW_VA,
    VARSY,

    KW_W,
    KW_WH,
    KW_WHI,
    KW_WHIL,
    WHILESY,
    
    IDENT,

    //Comment
    OPENCUR,
    COMMENT,
    CLOSECUR,
    //TODO: State for (*...*) comment style
};