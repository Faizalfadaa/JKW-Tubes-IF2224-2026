#include "reader.hpp"

int main(){
    Reader r;
    std::string path;

    std::cout << "Input path file: ";
    std::cin >> path;
    while (!r.open(path)){
        std::cout << "Input path file: ";
        std::cin >> path;
    }
    while (!r.isEOF()){
        std::cout << r.get(); //process every char here
        r.next();
    }
}