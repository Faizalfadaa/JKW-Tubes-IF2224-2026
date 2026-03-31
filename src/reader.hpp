#pragma once

#include <fstream>
#include <iostream>

using namespace std;


extern char cc; //(current char)
extern ifstream file;

ifstream openFile(string path);

char getCurrentChar();

char nextChar(ifstream& file);

bool isEOF(ifstream& file);
