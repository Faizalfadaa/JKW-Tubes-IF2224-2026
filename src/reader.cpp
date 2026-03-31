#include "reader.hpp"

using namespace std;

bool Reader::open(string path){
    this->file = ifstream(path, ios::in);
    if (!file){
        cout << "Gagal membaca file\n";
        return false;
    }
    next();
    return true;
}

char Reader::get(){
    return cc;
}

char Reader::peek(){
    int c = file.peek();

    //Handle error jika mencapai akhir file
    if (file.eof() || c == -1){
        return '\0';
    }
    return char(c);
}

char Reader::next(){
    int c = file.get();
    //Handle error jika mencapai EOF
    if (c == -1){
        return '\0';
    }
    cc = char(c);
    
    return cc;
}

bool Reader::isEOF(){
    return file.eof();
}