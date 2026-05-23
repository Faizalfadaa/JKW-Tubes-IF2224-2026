#include "symboltable.hpp"
#include "../utils/exception.hpp"

using namespace std;

SymbolTable::SymbolTable(){
    //Inisialisasi Symbol Table
    currentLevel = 0;
    lastTab = NULL;
    currentBlock = NULL;
    tab = vector<TabEntry>();
    atab = vector<ATabEntry>();
    btab = vector<BTabEntry>();

    //Insert block global
    btab.push_back(BTabEntry(0, NULL, NULL, 0, 0));
    currentBlock = &btab.front();

    //Insert predefined identifier
    insertTab("AND"    , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("ARRAY"  , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("BEGIN"  , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("CASE"   , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("CONST"  , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("DIV"    , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("DOWNTO" , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("DO"     , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("ELSE"   , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("END"    , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("FOR"    , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("FUNCTION", SymbolType::KEYWORD, BaseType::VOID);
    insertTab("IF"     , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("MOD"    , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("NOT"    , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("OF"     , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("OR"     , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("PROCEDURE", SymbolType::KEYWORD, BaseType::VOID);
    insertTab("PROGRAM", SymbolType::KEYWORD, BaseType::VOID);
    insertTab("RECORD" , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("REPEAT" , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("INTEGER", SymbolType::TYPE   , BaseType::INTEGER);
    insertTab("REAL"   , SymbolType::TYPE   , BaseType::REAL);
    insertTab("BOOLEAN", SymbolType::TYPE   , BaseType::BOOLEAN);
    insertTab("CHAR"   , SymbolType::TYPE   , BaseType::CHAR);
    insertTab("STRING" , SymbolType::TYPE   , BaseType::STRING);
    insertTab("THEN"   , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("TO"     , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("TYPE"   , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("UNTIL"  , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("VAR"    , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("WHILE"  , SymbolType::KEYWORD, BaseType::VOID);
    insertTab("TRUE"  , SymbolType::CONSTANT, BaseType::BOOLEAN);
    insertTab("FALSE"  , SymbolType::CONSTANT, BaseType::BOOLEAN);
}

    TabEntry* SymbolTable::insertTab(const string& name, SymbolType object, BaseType type, bool nrm){
        if (existsCurrentLevel(name)){
            throw IdentRedeclarationError(name);
        }
        
        tab.push_back(TabEntry(
            name,
            lastTab,
            object,
            type,
            NULL,
            nrm,
            currentLevel,
            0 //TODO: adr
        ));
        lastTab = &tab.back();

        //TODO: tambah ukuran block
    }

    ATabEntry* SymbolTable::insertATab(BaseType xtype, BaseType etype, int low, int high){
        int elsz = 0; //TODO: getSize(etype);
        atab.push_back(ATabEntry(
            atab.size(),
            xtype,
            etype,
            &tab.back(), //TODO: Harus selalu dipanggil setelah insert
            low,
            high,
            elsz,
            0
        ));
    }

    BTabEntry* SymbolTable::insertBTab(){
        int psze = 0; //TODO: calculate param size
        btab.push_back(BTabEntry(
            btab.size(),
            &tab.back(),
            &tab.back(),
            psze,
            0
        ));
    }

    TabEntry* SymbolTable::lookup(const string& name){
        TabEntry* next = &tab.back(); //FIXME: kasus tab.back setelah leaveScope?
        while (next != NULL){
            if (next->identifier == name){
                return next;
            }
            next = next->link;
        }
        return NULL;
    }

    bool SymbolTable::existsCurrentLevel(const string& name){
        TabEntry* next = &tab.back();
        while (next != NULL && next->lev == currentLevel){
            if (next->identifier == name){
                return false;
            }
            next = next->link;
        }
        return true;
    }

    void SymbolTable::enterScope(){
        currentLevel++;
        lastTab = &tab.back(); //FIXME: lastTab??
    }

    void SymbolTable::leaveScope(){
        if (currentLevel == 0){
            throw SymbolTableError("Can't leave global scope.");
        }
        currentLevel--;
        lastTab = &tab.back(); //FIXME: lastTab??
    }