#include "symboltable.hpp"
#include "../utils/exception.hpp"
#include "../utils/utils.hpp"

using namespace std;

TEntry::~TEntry() = default;

SymbolTable::SymbolTable(){
    //Inisialisasi Symbol Table
    currentLevel = 0;
    nextAdr = 0;
    lastTab = nullptr;
    currentBlock = nullptr;

    tab = vector<TabEntry>();
    atab = vector<ATabEntry>();
    btab = vector<BTabEntry>();

    scopeLast.clear();
    blockStack.clear();

    //Insert block global
    btab.push_back(BTabEntry(0, nullptr, nullptr, 0, 0));
    currentBlock = &btab.back();

    //Inisialisasi scope
    scopeLast.push_back(nullptr);
    blockStack.push_back(currentBlock);

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
    insertTab("READ", SymbolType::PROCEDURE, BaseType::VOID);
    insertTab("READLN", SymbolType::PROCEDURE, BaseType::VOID);
    insertTab("WRITE", SymbolType::PROCEDURE, BaseType::VOID);
    insertTab("WRITELN", SymbolType::PROCEDURE, BaseType::VOID);
}

int SymbolTable::getSize(BaseType type){
    switch (type) {
        case BaseType::INTEGER:
            return 1;
        case BaseType::REAL:
            return 1;
        case BaseType::BOOLEAN:
            return 1;
        case BaseType::CHAR:
            return 1;
        case BaseType::STRING:
            return 1;
        case BaseType::ARRAY:
            return 1;
        case BaseType::RECORD:
            return 1;
        default:
            return 0;
    }
}

TabEntry* SymbolTable::insertTab(const string& name, SymbolType object, BaseType type, bool nrm, TEntry* ref){
    //Ubah name agar case insensitive
    string nname = toUpper(name);

    if (existsCurrentLevel(nname)){
        throw IdentRedeclarationError(name);
    }

    //Hitung adr
    int adr = 0;
    if (object == SymbolType::VARIABLE){
        adr = nextAdr;
        nextAdr += getSize(type);
    }

    TabEntry* previousInSameScope = scopeLast.at(currentLevel);

    tab.push_back(TabEntry(
        nname,
        previousInSameScope,
        object,
        type,
        ref,
        nrm,
        currentLevel,
        adr
    ));

    TabEntry* inserted  = &tab.back();
    scopeLast.at(currentLevel) = inserted;
    lastTab = inserted;

    if (currentBlock) {
        currentBlock->last = inserted;
        if (object == SymbolType::VARIABLE) {
            currentBlock->vsze += getSize(type);
        }
    }

    return inserted;
}

ATabEntry* SymbolTable::insertATab(BaseType xtype, BaseType etype, int low, int high, TEntry* eref){
    int elsz = getSize(etype);
    int count = 0;
    if (high >= low) {
        count = high - low + 1;
    }
    int size = count * elsz;

    atab.push_back(ATabEntry(
        atab.size(),
        xtype,
        etype,
        eref,
        low,
        high,
        elsz,
        size
    ));

    return &atab.back();
}

BTabEntry* SymbolTable::insertBTab(){
    btab.push_back(BTabEntry(
        btab.size(),
        &tab.back(),
        nullptr,
        0,
        0
    ));

    currentBlock = &btab.back();

    if (!blockStack.empty()){
        blockStack.back() = currentBlock;
    }
    else{
        blockStack.push_back(currentBlock);
    }

    return currentBlock;
}

TabEntry* SymbolTable::lookup(const string& name){
    string nname = toUpper(name);

    for (int level = currentLevel; level >= 0; --level) {
        TabEntry* current = scopeLast.at(level);
        while (current != nullptr) {
            if (current->identifier == nname) {
                return current;
            }
            current = current->link;
        }
    }

    return nullptr;
}

bool SymbolTable::existsCurrentLevel(const string& name){
    string nname = toUpper(name);
    TabEntry* current = scopeLast.at(currentLevel);

    while (current != nullptr) {
        if (current->identifier == nname) {
            return true;
        }
        current = current->link;
    }

    return false;
}

void SymbolTable::enterScope(){
    currentLevel++;
    scopeLast.push_back(nullptr);

    btab.push_back(BTabEntry(
    btab.size(),
    nullptr,
    nullptr,
    0,
    0
    ));

    currentBlock = &btab.back();
    blockStack.push_back(currentBlock);
    lastTab = nullptr;
    nextAdr = 0;
}

void SymbolTable::leaveScope(){
    if (currentLevel == 0){
        throw SymbolTableError("Can't leave global scope.");
    }

    scopeLast.pop_back();
    blockStack.pop_back();

    currentLevel--;
    currentBlock = blockStack.empty() ? nullptr : blockStack.back();
    lastTab = scopeLast.at(currentLevel);
    nextAdr = currentBlock ? currentBlock->vsze : 0;
}

int SymbolTable::getCurrentLevel(){
    return currentLevel;
}

int SymbolTable::indexOf(const TabEntry* entry){
    int idx = 0;
    for (const auto& item : tab) {
        if (&item == entry) {
            return idx;
        }
        idx++;
    }

    return -1;
}

int SymbolTable::indexOf(const ATabEntry* entry){
    int idx = 0;
    for (const auto& item : atab) {
        if (&item == entry) {
            return idx;
        }
        idx++;
    }

    return -1;
}

int SymbolTable::indexOf(const BTabEntry* entry){
    int idx = 0;
    for (const auto& item : btab) {
        if (&item == entry) {
            return idx;
        }
        idx++;
    }

    return -1;
}

BaseType SymbolTable::toBaseType(const string& str){
    string normalized = toUpper(str);

    if (normalized == "INTEGER"){
        return BaseType::INTEGER;
    }
    else if (normalized == "REAL"){
        return BaseType::REAL;
    }
    else if (normalized == "BOOLEAN"){
        return BaseType::BOOLEAN;
    }
    else if (normalized == "CHAR"){
        return BaseType::CHAR;
    }
    else if (normalized == "STRING"){
        return BaseType::STRING;
    }
    else{
        return BaseType::UNKNOWN;
    }
}

string SymbolTable::toString(SymbolType type){
    switch (type) {
        case SymbolType::KEYWORD:   return "keyword";
        case SymbolType::CONSTANT:  return "constant";
        case SymbolType::VARIABLE:  return "variable";
        case SymbolType::TYPE:      return "type";
        case SymbolType::PROCEDURE: return "procedure";
        case SymbolType::FUNCTION:  return "function";
        default:                    return "unknown";
    }
}

string SymbolTable::toString(BaseType type){
    switch (type) {
        case BaseType::UNKNOWN:  return "unknown";
        case BaseType::VOID:     return "void";
        case BaseType::INTEGER:  return "integer";
        case BaseType::REAL:     return "real";
        case BaseType::BOOLEAN:  return "boolean";
        case BaseType::CHAR:     return "char";
        case BaseType::STRING:   return "string";
        case BaseType::ARRAY:    return "array";
        case BaseType::RECORD:   return "record";
        case BaseType::SUBRANGE: return "subrange";
        case BaseType::ENUM:     return "enum";
        default:                 return "unknown";
    }
}

const vector<TabEntry>& SymbolTable::getTab(){
    return tab;
}

const vector<ATabEntry>& SymbolTable::getATab(){
    return atab;
}

const vector<BTabEntry>& SymbolTable::getBTab(){
    return btab;
}

string SymbolTable::printTab(){
    ostringstream oss;

    oss << "===== TAB =====\n";
    oss << left
        << setw(5)  << "idx"
        << setw(18) << "identifier"
        << setw(12) << "obj"
        << setw(12) << "type"
        << setw(8)  << "ref"
        << setw(6)  << "nrm"
        << setw(6)  << "lev"
        << setw(6)  << "adr"
        << setw(6)  << "link"
        << "\n";

    oss << string(79, '-') << "\n";

    for (size_t i = 0; i < tab.size(); ++i){
        TabEntry& entry = tab[i];

        oss << left
            << setw(5)  << i
            << setw(18) << entry.identifier
            << setw(12) << SymbolTable::toString(entry.obj)
            << setw(12) << SymbolTable::toString(entry.type)
            << setw(8)  << (entry.ref ? "yes" : "null")
            << setw(6)  << (entry.nrm ? 1 : 0)
            << setw(6)  << entry.lev
            << setw(6)  << entry.adr
            << setw(6)  << indexOf(entry.link)
            << "\n";
    }

    return oss.str();
}

string SymbolTable::printATab(){
    ostringstream oss;

    oss << "===== ATAB =====\n";
    oss << left
        << setw(8)  << "idx"
        << setw(12) << "xtyp"
        << setw(12) << "etyp"
        << setw(8)  << "eref"
        << setw(8)  << "low"
        << setw(8)  << "high"
        << setw(8)  << "elsz"
        << setw(8)  << "size"
        << "\n";

    oss << string(72, '-') << "\n";

    for (size_t i = 0; i < atab.size(); ++i){
        ATabEntry& entry = atab[i];

        oss << left
            << setw(8)  << i
            << setw(12) << SymbolTable::toString(entry.xtyp)
            << setw(12) << SymbolTable::toString(entry.etyp)
            << setw(8)  << (entry.eref ? "yes" : "null")
            << setw(8)  << entry.low
            << setw(8)  << entry.high
            << setw(8)  << entry.elsz
            << setw(8)  << entry.size
            << "\n";
    }

    return oss.str();
}

string SymbolTable::printBTab(){
    ostringstream oss;

    oss << "===== BTAB =====\n";
    oss << left
        << setw(8) << "idx"
        << setw(8) << "last"
        << setw(8) << "lpar"
        << setw(8) << "psze"
        << setw(8) << "vsze"
        << "\n";

    oss << string(40, '-') << "\n";

    for (size_t i = 0; i < btab.size(); ++i){
        BTabEntry& entry = btab[i];

        oss << left
            << setw(8) << i
            << setw(8) << indexOf(entry.last)
            << setw(8) << indexOf(entry.lpar)
            << setw(8) << entry.psze
            << setw(8) << entry.vsze
            << "\n";
    }

    return oss.str();
}

string SymbolTable::printAll(){
    ostringstream oss;

    oss << printTab() << "\n";
    oss << printATab() << "\n";
    oss << printBTab();

    return oss.str();
}
