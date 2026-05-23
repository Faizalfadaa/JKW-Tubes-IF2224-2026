#pragma once

#include <deque>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

//Enum untuk SymbolType
enum class SymbolType {
    KEYWORD,
    CONSTANT,
    VARIABLE,
    TYPE,
    PROCEDURE,
    FUNCTION
};

//Enum untuk tipe dasar
enum class BaseType {
    UNKNOWN, VOID,
    INTEGER, REAL, BOOLEAN, CHAR, STRING,
    ARRAY, RECORD, SUBRANGE, ENUM
};

//Parent dari tab entry
class TEntry {
public:
    virtual ~TEntry() = 0;
};

//Elemen dari tab
class TabEntry : public TEntry {
public:
    std::string identifier;
    TabEntry* link; 
    SymbolType obj;
    BaseType type;
    TEntry* ref;
    bool nrm;
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
    TabEntry(std::string identifier, TabEntry* link,SymbolType obj, BaseType type, TEntry* ref, bool nrm, int lev, int adr):
        identifier(identifier), link(link), obj(obj), type(type), ref(ref), nrm(nrm), lev(lev), adr(adr){}
};

//Elemen dari atab
class ATabEntry : public TEntry {
public:
    int arrays;
    BaseType xtyp;
    BaseType etyp;
    TEntry* eref;
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
    ATabEntry(int arrays, BaseType xtyp, BaseType etyp, TEntry* ref, int low, int high, int elsz, int size):
        arrays(arrays), xtyp(xtyp), etyp(etyp), eref(ref), low(low), high(high), elsz(elsz), size(size){}
};

//Elemen dari btab
class BTabEntry : public TEntry {
public:
    int blocks;
    TabEntry* last;
    TabEntry* lpar;
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
    BTabEntry(int blocks, TabEntry* last, TabEntry* lpar, int psze, int vsze):
        blocks(blocks), last(last), lpar(lpar), psze(psze), vsze(vsze){}
};

class SymbolTable {
private:
    //Atribut
    int currentLevel;
    int nextAdr;
    TabEntry* lastTab;
    BTabEntry* currentBlock;

    std::vector<TabEntry> tab;
    std::vector<ATabEntry> atab;
    std::vector<BTabEntry> btab;

    std::vector<TabEntry*> scopeLast;
    std::vector<BTabEntry*> blockStack;

    int getSize(BaseType type);

public:
    SymbolTable();

    TabEntry* insertTab(const std::string& name, SymbolType object, BaseType type, bool nrm = true, TEntry* ref = nullptr);
    ATabEntry* insertATab(BaseType xtype, BaseType etype, int low, int high, TEntry* eref = nullptr);
    BTabEntry* insertBTab();

    TabEntry* lookup(const std::string& name);

    bool existsCurrentLevel(const std::string& name);

    void enterScope();
    void leaveScope();

    int getCurrentLevel();
    int indexOf(const TabEntry* entry);
    int indexOf(const ATabEntry* entry);
    int indexOf(const BTabEntry* entry);

    BaseType toBaseType(const std::string& string);
    static std::string toString(SymbolType type);
    static std::string toString(BaseType type);

    const std::vector<TabEntry>& getTab();
    const std::vector<ATabEntry>& getATab();
    const std::vector<BTabEntry>& getBTab();

    std::string printTab();
    std::string printATab();
    std::string printBTab();
    std::string printAll();
};