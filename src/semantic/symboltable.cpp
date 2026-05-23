#include "symboltable.hpp"
#include "../utils/exception.hpp"

SymbolTable::SymbolTable(){
    currentLevel = 0;
    //push null??
    tab.push_back(TabEntry("AND"    , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("ARRAY"  , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("BEGIN"  , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("CASE"   , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("CONST"  , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("DIV"    , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("DOWNTO" , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("DO"     , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("ELSE"   , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("END"    , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("FOR"    , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("FUNCTION", NULL, SymbolType::KEYWORD, BaseType::VOID   , NULL, 1, 0, 0));
    tab.push_back(TabEntry("IF"     , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("MOD"    , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("NOT"    , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("OF"     , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("OR"     , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("PROCEDURE", NULL, SymbolType::KEYWORD, BaseType::VOID  , NULL, 1, 0, 0));
    tab.push_back(TabEntry("PROGRAM", NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("RECORD" , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("REPEAT" , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("INTEGER", NULL, SymbolType::TYPE   , BaseType::INTEGER , NULL, 1, 0, 0));
    tab.push_back(TabEntry("REAL"   , NULL, SymbolType::TYPE   , BaseType::REAL    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("BOOLEAN", NULL, SymbolType::TYPE   , BaseType::BOOLEAN , NULL, 1, 0, 0));
    tab.push_back(TabEntry("CHAR"   , NULL, SymbolType::TYPE   , BaseType::CHAR    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("STRING" , NULL, SymbolType::TYPE   , BaseType::STRING  , NULL, 1, 0, 0));
    tab.push_back(TabEntry("THEN"   , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("TO"     , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("TYPE"   , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("UNTIL"  , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("VAR"    , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("WHILE"  , NULL, SymbolType::KEYWORD, BaseType::VOID    , NULL, 1, 0, 0));
    tab.push_back(TabEntry("TRUE"  , NULL, SymbolType::CONSTANT, BaseType::BOOLEAN , NULL, 1, 0, 0));
    tab.push_back(TabEntry("FALSE"  , NULL, SymbolType::CONSTANT, BaseType::BOOLEAN, NULL, 1, 0, 0));
}

    void SymbolTable::insert(const std::string& name, SymbolType object, BaseType type, bool nrm){
        if (existsCurrentLevel(name)){
            throw IdentRedeclarationError(name);
        }
        
        TabEntry* ref = NULL;

        if (type == BaseType::ARRAY){
            insertATab(ATabEntry(
                atab.size(),
                type, //TODO: xtype
                type,
                lastTab,
                0, //TODO low
                10, //TODO high
                4, //TDO elsz,
                10 //TODO size
            ));
        }
        else if (type == BaseType::RECORD || object == SymbolType::FUNCTION || object == SymbolType::PROCEDURE){
            //TODO
        }

        insertTab(TabEntry(
            name,
            lastTab,
            object,
            type,
            ref,
            nrm,
            currentLevel,
            0 //TODO: adr
        ));
        lastTab = &tab.back();
    }

    int SymbolTable::insertTab(const TabEntry& entry){
        if (existsCurrentLevel(entry.identifier)){
            throw IdentRedeclarationError(entry.identifier);
        }

        tab.push_back(entry);
    }

    int SymbolTable::insertATab(const ATabEntry& entry){
        atab.push_back(entry);
    }

    int SymbolTable::insertBTab(const BTabEntry& entry){
        btab.push_back(entry);
    }

    TabEntry* SymbolTable::lookup(const std::string& name){
        TabEntry* next = &tab.back(); //FIXME: kasus tab.back setelah leaveScope?
        while (next != NULL){
            if (next->identifier == name){
                return next;
            }
            next = next->link;
        }
        return NULL;
    }

    bool SymbolTable::existsCurrentLevel(const std::string& name){
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
        currentLevel--;
        lastTab = &tab.back(); //FIXME: lastTab??
    }