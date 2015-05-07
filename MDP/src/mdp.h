#ifndef MDP_H
#define MDP_H

#include "database.h"
#include <memory>
#include <vector>
#include <string>

namespace mdp {
class MDP
{
private:
    std::shared_ptr<database::Database> db;
    size_t m_minSlack;
    size_t m_maxSlack;
public:
    MDP(std::shared_ptr<database::Database> db, size_t minSlack, size_t maxSlack);
    ~MDP();

    void run(std::string const &outpuFile);
    void runEmbeddings(std::string const &outpuFile);
    void runPatterns(std::string const &outpuFile);
    void runPatternsSeq(std::string const &outpuFile);
    void runPatternsTime(std::string const &outpuFile);
private:
    std::vector<size_t> getSlacks(std::vector<std::string> const &sequences);
};
}

#endif // MDP_H
