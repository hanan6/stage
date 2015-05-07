#include "database.h"
#include <fstream>

using namespace std;

namespace mdp { namespace database {

void Database::save(std::string const &fileName) {
    ofstream file(fileName);

    for(string const &name: classes) {
        for(size_t i = 0; i < base[name].second.size(); ++i) {
            file << name  << " " << base[name].first[i] << " " << base[name].second[i] << endl;
        }
    }
}

}}
