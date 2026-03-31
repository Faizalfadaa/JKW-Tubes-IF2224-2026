#include "reader.hpp"

char cc; //(current char)

ifstream openFile(string path){
    ifstream file(path, ios::in);
    if (!file){
        cout << "Gagal membaca file\n";
        return NULL;
    }
    file.get(cc);
    return file;
}

char getCurrentChar(){
    return cc;
}

char nextChar(ifstream& file){
    file.get(cc);
    if (file.eof()){
        file.close();
    }
    return cc;
}

bool isEOF(ifstream& file){
    return file.eof();
}