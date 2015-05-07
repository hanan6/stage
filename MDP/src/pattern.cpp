#include "pattern.h"

#include <algorithm>
#include <sstream>
#include <iterator>

using namespace std;




Pattern::Pattern(Pattern&& src)
: m_solids(move(src.m_solids))
, m_sections(move(src.m_sections)), m_slack(move(src.m_slack)), m_size(move(src.m_size))
, m_tmp(move(src.m_tmp))
{}

Pattern::Pattern()
: m_solids(), m_sections(), m_slack(), m_size(), m_tmp()
{}

Pattern::Pattern(const string& src)
: m_solids(), m_sections(), m_slack(), m_size(), m_tmp()
{
    __build(src.cbegin(), src.cend());
}

Pattern::Pattern(const Pattern& src)
: m_solids(src.m_solids), m_sections(src.m_sections), m_slack(src.m_slack), m_size(src.m_size)
, m_tmp(src.m_tmp)
{}


Pattern& Pattern::operator=(Pattern&& rhs) {
    if (this != &rhs) {
        m_solids   = move(rhs.m_solids);
        m_sections = move(rhs.m_sections);
        m_slack    = move(rhs.m_slack);
        m_size     = move(rhs.m_size);
        m_tmp      = move(rhs.m_tmp);
    }
    return *this;
}

Pattern& Pattern::operator=(const Pattern& rhs) {
    if (this != &rhs) {
        m_solids   = rhs.m_solids;
        m_sections = rhs.m_sections;
        m_slack    = rhs.m_slack;
        m_size     = rhs.m_size;
        m_tmp      = rhs.m_tmp;
    }
    return *this;
}

Pattern::~Pattern()
{}

bool Pattern::search(Pattern const &src) const {
    // cuts
    if(solids() == 2 && src.solids() == 2 && size() != src.size()) return false;
    if(size() == src.size() && slack() < src.slack()) return false;

    size_t     adv     = 0;
    auto       it_src  = src.begin();
    auto const end_src = src.end();
    while(it_src != end_src && size() <= src.size() - adv) {
        auto it_beg = it_src;
        auto it     = begin();
        bool found  = true;
        while(it != end() && found) {
            if(it_beg->first != it->first) {
                found = false;
                break;
            }
            // increment it bye one, then increment it_beg as much as necessary
            found = it.incrementIteratorThen(it_beg);
        }

        if(found)
            return true;
        ++it_src;
        adv += 1 + it_src->second;
    }
    return false;
}

bool Pattern::search(std::string const &src) const {
    size_t i_src = 0;
    while(size() <= src.size() - i_src) {
        size_t   i_beg = i_src;
        bool found = true;
        for(auto it = begin(); it != end() && found; ++it) {
            i_beg += it->second;
            found = src[i_beg++] == it->first;
        }
        if(found)
            return true;
        i_src++;
    }
    return false;
}

size_t Pattern::search(std::string const &src, size_t start) const {
    size_t i_src = start;
    while(size() <= src.size() - i_src) {
        size_t   i_beg = i_src;
        bool found = true;
        for(auto it = begin(); it != end() && found; ++it) {
            i_beg += it->second;
            found = src[i_beg++] == it->first;
        }
        if(found)
            return i_src;
        i_src++;
    }
    return string::npos;
}

bool Pattern::operator==(const Pattern& rhs) const {
    return m_tmp == rhs.m_tmp;
}

Pattern::size_type Pattern::slack(void) const {
    return m_slack;
}

Pattern::size_type Pattern::size(void) const {
    return m_size;
}

Pattern::size_type Pattern::solids(void) const {
    return m_solids.size();
}

// TO-DO
vector<Pattern> Pattern::split_by_slack(size_t slack) const {
    vector<Pattern> p;
    return p;
}

string Pattern::toString(void) const {
    ostringstream os;
    os << *this;

    return os.str();
}

Pattern::iterator Pattern::begin(void) const {
    return iterator_impl(this);
}

Pattern::iterator Pattern::end(void) const {
    return iterator_impl(this, false);
}

Pattern::const_iterator Pattern::cbegin(void) const {
    return iterator_impl(this);
}

Pattern::const_iterator Pattern::cend(void) const {
    return iterator_impl(this, false);
}

void swap(Pattern& lhs, Pattern& rhs) {
    using std::swap;

    swap(lhs.m_solids, rhs.m_solids);
    swap(lhs.m_sections, rhs.m_sections);
    swap(lhs.m_slack, rhs.m_slack);
    swap(lhs.m_size, rhs.m_size);
    swap(lhs.m_tmp, rhs.m_tmp);
}

ostream& operator<<(ostream& os, const Pattern& obj) {
    for(auto v: obj) {
        fill_n(ostream_iterator<char>(os), v.second, '.');
        os << v.first;
    }
    return os;
}
