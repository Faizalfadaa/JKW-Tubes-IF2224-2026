#include "reader.hpp"

int main(){
    ifstream file = openFile("test/data.txt");
    while (!isEOF(file)){
        // cout << cc; //process every char here
        nextChar(file);
    }
}