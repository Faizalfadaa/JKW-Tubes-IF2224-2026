#pragma once

#include <vector>
#include <string>

//Enum untuk SymbolType
enum class SymbolType {
    KEYWORD, //untuk keyword
    CONSTANT,
    VARIABLE,
    TYPE,
    PROCEDURE,
    FUNCTION
    //TODO: tambah
};

//Enum untuk tipe dasar
enum class BaseType {
    NOTYPE, //untuk void
    INT,
    BOOL,
    CHAR,
    STRING,
    REAL,
    ARRAY,
    RECORD,
    SUBRANGE,
    ENUM
    //TODO: tambah
};

//Elemen dari tab
class TabEntry {
public:
    std::string identifier;
    int link; //atau pointer
    SymbolType obj;
    BaseType type;
    int ref; //atau pointer
    int nrm;
    int lev;
    int adr;

    /**
    @param identifier 
    Nama identifier (misalnya nama variabel, konstanta, tipe, prosedur, fungsi).
    @param link
    Pointer/indeks ke identifier sebelumnya dalam scope yang sama. Digunakan untuk manajemen scope (linked list per blok).
    @param obj
    Kelas objek yang dienumerasi: konstanta, variabel, tipe, prosedur, fungsi, dll.
    @param type
    Tipe dasar dari identifier, misalnya: integer, boolean, char, real, array, record, dll. Biasanya berupa kode numerik.
    @param ref
    Pointer/indeks ke tabel lain jika tipe adalah komposit (array/record). Mengarah ke atab (array table) atau btab (record/procedure block).
    @param nrm
    Menandai apakah identifier adalah variabel normal (=1) atau parameter by-reference (var parameter) (=0).
    @param lev
    Tingkat lexical level tempat identifier dideklarasikan (0 = global, 1 = dalam prosedur, 2 = dalam prosedur di dalam prosedur, dst).
    @param adr
    Makna tergantung jenis objek: offset variabel di stack frame, nilai konstanta, offset field record, alamat prosedur, atau ukuran/penanda lain.
     */
    TabEntry(std::string identifier, int link,SymbolType obj, BaseType type, int ref, int nrm, int lev, int adr):
        identifier(identifier), link(link), obj(obj), type(type), ref(ref), nrm(nrm), lev(lev), adr(adr){}
};

//Elemen dari atab
class ATabEntry {
public:
    int arrays; //template??
    BaseType xtyp;
    BaseType etyp;
    int eref; //atau pointer
    int low;
    int high;
    int elsz;
    int size;

    /**
    @param arrays
    Indeks entri array
    @param xtyp
    Tipe indeks array (misalnya integer). Berupa kode tipe dari tabel tab.
    @param etyp
    Tipe elemen array (misalnya integer). Berupa kode tipe dari tabel tab.
    @param eref
    Pointer/indeks ke detail tipe elemen jika elemen adalah tipe komposit (misalnya array dalam array, atau record). Mengarah ke atab atau btab.
    @param low
    Batas bawah indeks array (misalnya 1 pada array[1..10] atau 0 pada array[0..15]).
    @param high
    Batas atas indeks array.
    @param elsz
    Ukuran satu elemen array (dalam byte/unit memori).
    @param size
    Total ukuran array
     */
    ATabEntry(int arrays, BaseType xtyp, BaseType etyp, int eref, int low, int high, int elsz, int size):
        arrays(arrays), xtyp(xtyp), etyp(etyp), eref(eref), low(low), high(high), elsz(elsz), size(size){}
};

//Elemen dari btab
class BTabEntry {
public:
    int blocks;
    int last; //atau pointer
    int lpar; //atau pointer
    int psze;
    int vsze;

    /**
    @param blocks
    Indeks entri block (setiap block mewakili prosedur, fungsi, atau record type definition).
    @param last
    Pointer/indeks ke identifier terakhir yang dideklarasikan di dalam block tersebut (menghubungkan field record, parameter, atau variabel lokal).
    @param lpar
    Pointer/indeks ke parameter terakhir dari prosedur/fungsi pada block tersebut. Jika block adalah record, nilainya 0.
    @param psze
    Total ukuran parameter block  (dalam byte/unit memori).
    @param vsze
    Total ukuran variabel lokal block (dalam byte/unit memori)
     */
    BTabEntry(int blocks, int last, int lpar, int psze, int vsze):
        blocks(blocks), last(last), lpar(lpar), psze(psze), vsze(vsze){}
};

class SymbolTable {
private:
    std::vector<TabEntry> tab;
    std::vector<ATabEntry> atab;
    std::vector<BTabEntry> btab;

public:
    SymbolTable(){
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

    
};