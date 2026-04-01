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

    //Ubah cc menjadi char
    return char(c);
}

char Reader::next(){
    int c = file.get();

    //Handle error jika mencapai EOF
    if (c == -1){
        return '\0';
    }

    //Update current character
    cc = char(c);

    // //Skip space dan newline
    // if (cc == ' ' || cc == '\n'){
    //     next();
    // }
    
    return cc;
}

bool Reader::isEOF(){
    return file.eof();
}