#include "utils.hpp"

using namespace std;

string toLower(const string& str) {
    string lower = str;
    for (char& c : lower) {
        c = tolower(c);
    }
    return lower;
}

string toUpper(const string& str) {
    string upper = str;
    for (char& c : upper) {
        c = toupper(c);
    }
    return upper;
}

string stripQuotes(const std::string& str) {
    if (str.size() >= 2 && str.front() == '\'' && str.back() == '\'') {
        return str.substr(1, str.size() - 2);
    }
    return str;
}