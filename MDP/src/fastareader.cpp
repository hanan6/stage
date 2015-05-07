#include "fastareader.h"

#include <fstream>  // ifstream
#include <iterator> // istream_iterator
#include <sstream>  // stringstream
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <stdlib.h>
#include <iostream>

using namespace std;
namespace mdp {namespace database {

std::string trim(std::string const& str)
{
    if(str.empty())
        return str;

        if(str.size() > 1) {
            std::size_t first = str.find_first_not_of(' ');
            std::size_t last  = str.find_last_not_of(' ');
            return str.substr(first, last-first+1);
        }
        if(str == " ")
            return "";
        return str;
}

bool FastaReader::nextLine(istream& is, string& str) {
    while(getline(is, str)) {
        if(!str.empty() && (*str.rbegin() == '\r' || *str.rbegin() == '\n')) {
            str.erase( str.length()-1, 1);
        }
        str = trim(str);
        size_t pos = str.find_first_not_of(" \t");
        if(pos != string::npos) return true;
    }
    return false;
}

bool FastaReader::isHeader(const string& str) {
    string tmp = str.substr(str.find_first_not_of(" \t"));
    return !tmp.empty() && tmp.front() == '>';
}

std::shared_ptr<Database> FastaReader::read(const string& file_name, std::unordered_set<std::string> dummies) {
    ifstream                   file(file_name);

    Base db;
    string         line;
    bool hasNextLine = nextLine(file, line);
    do {
        if(!isHeader(line)) {
            cerr << "Fichier invalide ! " << std::endl;
            exit(0);
        }
        string name  = line.substr(1, line.find_last_of('_') - 1);
        if(name.find("_cl") != std::string::npos) {
            name  = name.substr(0, name.find_last_of('_') - 1);
        }
        string nameClass = line.substr(line.find_last_of('_') + 1);
        Sequence seq;
        while((hasNextLine = nextLine(file, line)) && !isHeader(line)) {
            seq += line;
        }
        if(dummies.find(nameClass) == dummies.end()) {
            db[nameClass].first.push_back(name);
            db[nameClass].second.push_back(seq);
        }
    } while(hasNextLine);

    return make_shared<Database>(db);
}
}}
