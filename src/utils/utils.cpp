#include "utils.hpp"

using namespace std;

string toLower(const string& str) {
    string lower = str;
    for (char& c : lower) {
        c = tolower(c);
    }
    return lower;
}
