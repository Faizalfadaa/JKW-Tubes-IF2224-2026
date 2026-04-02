#pragma once

#include <fstream>
#include <iostream>

class Reader{
private:
    char cc; //current character
    std::ifstream file;
public:
    /** @return false jika gagal membaca file*/
    bool open(std::string path);

    /** @return char yang ditunjuk saat ini*/
    char get();

    /** @return char selanjutnya
     * @note Tidak boleh digunakan
    */
    // char peek();

    /** @brief Membaca char selanjutnya
     *  @note Harus stop sebelum EOF
     *  @return char yang baru dibaca
    */
    char next();

    /** @return true jika reader mencapai EOF*/
    bool isEOF();
};