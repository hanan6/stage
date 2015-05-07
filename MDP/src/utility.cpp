#include "utility.h"

#include <iostream>

using namespace std;

std::unordered_set<std::string> split(const std::string &s, char delim) {
    std::unordered_set<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.insert(item);
    }
    return elems;
}
