#include "symboltable.hpp"

SymbolTable::SymbolTable(){
    currentLevel = 0;
    tab.push_back(TabEntry("AND"    , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("ARRAY"  , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("BEGIN"  , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("CASE"   , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("CONST"  , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("DIV"    , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("DOWNTO" , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("DO"     , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("ELSE"   , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("END"    , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("FOR"    , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("FUNCTION", 0, SymbolType::KEYWORD, BaseType::NOTYPE , -1, 1, 0, 0));
    tab.push_back(TabEntry("IF"     , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("MOD"    , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("NOT"    , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("OF"     , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("OR"     , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("PROCEDURE", 0, SymbolType::KEYWORD, BaseType::NOTYPE, -1, 1, 0, 0));
    tab.push_back(TabEntry("PROGRAM", 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("RECORD" , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("REPEAT" , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("INTEGER", 0, SymbolType::TYPE   , BaseType::INT     , -1, 1, 0, 0));
    tab.push_back(TabEntry("REAL"   , 0, SymbolType::TYPE   , BaseType::REAL    , -1, 1, 0, 0));
    tab.push_back(TabEntry("BOOLEAN", 0, SymbolType::TYPE   , BaseType::BOOL    , -1, 1, 0, 0));
    tab.push_back(TabEntry("CHAR"   , 0, SymbolType::TYPE   , BaseType::CHAR    , -1, 1, 0, 0));
    tab.push_back(TabEntry("STRING" , 0, SymbolType::TYPE   , BaseType::STRING  , -1, 1, 0, 0));
    tab.push_back(TabEntry("THEN"   , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("TO"     , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("TYPE"   , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("UNTIL"  , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("VAR"    , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
    tab.push_back(TabEntry("WHILE"  , 0, SymbolType::KEYWORD, BaseType::NOTYPE  , -1, 1, 0, 0));
}

    int SymbolTable::insertTab(const TabEntry& entry){
        //TODO:
    }

    int SymbolTable::insertATab(const ATabEntry& entry){
        //TODO:
    }

    int SymbolTable::insertBTab(const BTabEntry& entry){
        //TODO:
    }

    int SymbolTable::lookup(std::string& name){
        //TODO:
    }

    bool SymbolTable::existsCurrentLevel(std::string& name){
        //TODO:
    }

    void SymbolTable::enterScope(){
        //TODO:
    }

    void SymbolTable::leaveScope(){
        //TODO:  
    }