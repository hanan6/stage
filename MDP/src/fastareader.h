#ifndef FASTA_READER_H
#define FASTA_READER_H

#include "database.h"
#include <string>
#include <istream>
#include <memory>
#include <unordered_set>

namespace mdp {namespace database {

class FastaReader {
private:
    static bool nextLine(std::istream& is, std::string& str);
    static bool isHeader(const std::string& str);
public:
    static std::shared_ptr<Database> read(std::string const &file_name, std::unordered_set<std::string> dummies);
};

}}

#endif
