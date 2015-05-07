#ifndef XMLWRITTER_H
#define XMLWRITTER_H

#include "matrix.h"
#include "pattern.h"
#include <vector>
#include <string>
#include <fstream>
#include <memory>

namespace mdp {
using std::move;
class XMLWritter
{
private:
    typedef std::shared_ptr<std::vector<Pattern>> ptr_vectorP;
    typedef std::shared_ptr<std::vector<std::string>>          ptr_vectorS;
    typedef std::shared_ptr<matrix<size_t>>       ptr_matrixS;
public:
    class Entry {
    private:
        std::string m_name    ; /*!< name of the entrye */
        size_t m_posSequences ; /*!< number of posititve sequences */
        size_t m_negSequences ; /*!< number of negative sequences */
        size_t m_posMinSize   ; /*!< size of the shortest posititve sequence */
        size_t m_posMaxSize   ; /*!< size of the longest posititve sequence */
        size_t m_negMinSize   ; /*!< size of the shortest negative sequence */
        size_t m_negMaxSize   ; /*!< size of the longest negative sequence */
        bool   m_satisfied    ; /*!< holds if there is a solution */
        ptr_vectorS m_names;      /*!< posititve sequences names */
        size_t m_card  ; /*!< cardinality of the motif */
        size_t m_excl  ; /*!< number of negative sequences excluded */
        size_t m_slack ;
        ptr_vectorP      m_patterns; /*!< set of patterns */
        ptr_matrixS       m_embeddings; /*!< posititve sequences first embedings */
    public:
        Entry
            ( std::string const &name
            , size_t posSequences, size_t negSequences
            , size_t posMinSize  , size_t posMaxSize
            , size_t negMinSize  , size_t negMaxSize
            , ptr_vectorS && names
            , size_t card, size_t excl, size_t slack
            , ptr_vectorP &&patterns
            , ptr_matrixS       &&embeddings
            )
        : m_name(name)
        , m_posSequences(posSequences), m_negSequences(negSequences)
        , m_posMinSize(posMinSize)    , m_posMaxSize(posMaxSize)
        , m_negMinSize(negMinSize)    , m_negMaxSize(negMaxSize)
        , m_satisfied(true)
        , m_names(move(names))
        , m_card(card), m_excl(excl), m_slack(slack)
        , m_patterns(move(patterns))
        , m_embeddings(move(embeddings))
        {}

        Entry
            ( std::string const &name
            , size_t posSequences, size_t negSequences
            , size_t posMinSize  , size_t posMaxSize
            , size_t negMinSize  , size_t negMaxSize
            , ptr_vectorS const &names
            , size_t card, size_t excl, size_t slack
            , ptr_vectorP const &patterns
            , ptr_matrixS const &embeddings
            )
        : m_name(name)
        , m_posSequences(posSequences), m_negSequences(negSequences)
        , m_posMinSize(posMinSize)    , m_posMaxSize(posMaxSize)
        , m_negMinSize(negMinSize)    , m_negMaxSize(negMaxSize)
        , m_satisfied(true)
        , m_names(names)
        , m_card(card), m_excl(excl), m_slack(slack)
        , m_patterns(patterns)
        , m_embeddings(embeddings)
        {}

        Entry(Entry &&e)
        : m_name(move(e.m_name))
        , m_posSequences(move(e.m_posSequences)), m_negSequences(move(e.m_negSequences))
        , m_posMinSize(move(e.m_posMinSize))    , m_posMaxSize(move(e.m_posMaxSize))
        , m_negMinSize(move(e.m_negMinSize))    , m_negMaxSize(move(e.m_negMaxSize))
        , m_satisfied(move(e.m_satisfied))
        , m_names(move(e.m_names))
        , m_card(move(e.m_card)), m_excl(move(e.m_excl)), m_slack(move(e.m_slack))
        , m_patterns(move(e.m_patterns))
        , m_embeddings(move(e.m_embeddings))
        {}

        Entry(Entry const &e)
        : m_name(e.m_name)
        , m_posSequences(e.m_posSequences), m_negSequences(e.m_negSequences)
        , m_posMinSize(e.m_posMinSize)    , m_posMaxSize(e.m_posMaxSize)
        , m_negMinSize(e.m_negMinSize)    , m_negMaxSize(e.m_negMaxSize)
        , m_satisfied(e.m_satisfied)
        , m_names(e.m_names)
        , m_card(e.m_card), m_excl(e.m_excl), m_slack(e.m_slack)
        , m_patterns(e.m_patterns)
        , m_embeddings(e.m_embeddings)
        {}

        Entry
            ( std::string const &name
            , size_t posSequences, size_t negSequences
            , size_t posMinSize  , size_t posMaxSize
            , size_t negMinSize  , size_t negMaxSize
            )
        : m_name(name)
        , m_posSequences(posSequences), m_negSequences(negSequences)
        , m_posMinSize(posMinSize)    , m_posMaxSize(posMaxSize)
        , m_negMinSize(negMinSize)    , m_negMaxSize(negMaxSize)
        , m_satisfied(false)
        {}

        void write(std::ostream &file, std::string const& startLine="") const {
            static std::string const tab = "\t";

            file << startLine; startEntry(file);
            file << startLine << tab; satisified(file);
            file << startLine << tab; posSequences(file);
            file << startLine << tab; negSequences(file);
            file << startLine << tab; posMinSize(file);
            file << startLine << tab; posMaxSize(file);
            file << startLine << tab; negMinSize(file);
            file << startLine << tab; negMaxSize(file);
            if(m_satisfied) {
                motif(file, startLine);
                embeddings(file, startLine);
            }
            endEntry(file);
        }
    private:
        void motif(std::ostream &file, std::string const& startLine="") const {
            static std::string const tab = "\t";
            file << startLine << tab; startMotif(file);
            file << startLine << tab << tab; card(file);
            file << startLine << tab << tab; excl(file);
            file << startLine << tab << tab; slack(file);
            file << startLine << tab << tab; startPatterns(file);
            for(Pattern const &p: *m_patterns) {
                file << startLine << tab << tab << tab; pattern(file, p);
            }
            file << startLine << tab; endPatterns(file);
            file << startLine << tab; endMotif(file);
        }

        void embeddings(std::ostream &file, std::string const& startLine="") const {
            static std::string const tab = "\t";
            file << startLine << tab; startEmbeddings(file);
            for(size_t s = 0; s < m_posSequences; ++s) {
                file << startLine << tab << tab;
                startSequence(file, s);
                embeddingsSequence(file, s);
                endSequence(file);
            }
            file << startLine << tab; endEmbeddings(file);
        }

        void embeddingsSequence(std::ostream &file, size_t s) const {
            for(size_t p = 0; p < m_card; ++p) {
                file << (*m_embeddings)(s,p) << " ";
            }
        }

        void startEntry(std::ostream &file) const {
            file << "<class name=" << "\"" << m_name << "\">" << std::endl;
        }

       void endEntry(std::ostream &file) const {
             file << "</class>" << std::endl;
       }

       void startEmbeddings(std::ostream &file) const {
            file << "<sequences>" << std::endl;
       }

       void endEmbeddings(std::ostream &file) const {
           file << "</sequences>" << std::endl;
       }

       void startSequence(std::ostream &file, size_t s) const {
           file << "<sequences name=\"" << (*m_names)[s] << "\">";
       }

       void endSequence(std::ostream &file) const {
           file << "</sequences>" << std::endl;
       }

       void satisified(std::ostream &file) const {
           file << "<satisfied val =\"";
           if(m_satisfied)
               file << "true";
           else
               file << "false";
           file << "\" />" << std::endl;
       }

       void posSequences(std::ostream &file) const {
           file << "<nbPos val=\"" << m_posSequences << "\" />" << std::endl;
       }

       void negSequences(std::ostream &file) const {
           file << "<nbNeg val=\"" << m_negSequences << "\" />" << std::endl;
       }

       void posMinSize(std::ostream &file) const {
           file << "<shortesPos val=\"" << m_posMinSize << "\" />" << std::endl;
       }

       void posMaxSize(std::ostream &file) const {
           file << "<longestPos val=\"" << m_posMaxSize << "\" />" << std::endl;
       }

       void negMinSize(std::ostream &file) const {
           file << "<shortestNeg val=\"" << m_negMinSize << "\" />" << std::endl;
       }

       void negMaxSize(std::ostream &file) const {
           file << "<longestNeg val=\"" << m_negMaxSize << "\" />" << std::endl;
       }

       void startMotif(std::ostream &file) const {
           file << "<motif>" << std::endl;
       }

       void endMotif(std::ostream &file) const {
           file << "</motif>" << std::endl;
       }

       void startPatterns(std::ostream &file) const {
           file << "<patterns>" << std::endl;
       }

       void endPatterns(std::ostream &file) const {
           file << "</patterns>" << std::endl;
       }

       void pattern(std::ostream &file, Pattern const &p) const {
           file << "<pattern>" << p << "</pattern>" << std::endl;
       }

       void excl(std::ostream &file) const {
           file << "<nexcl val=\"" << m_excl << "\" />" << std::endl;
       }

       void card(std::ostream &file) const {
           file << "<card val=\"" << m_card << "\" />" << std::endl;
       }

       void slack(std::ostream &file) const {
           file << "<slack val=\"" << m_slack << "\" />" << std::endl;
       }

    };
private:
    std::vector<Entry> m_entries;

public:
    XMLWritter() {

    }

    ~XMLWritter() {

    }

    void addEntry(Entry const &e) {
        m_entries.push_back(e);
    }

    void addEntry(Entry &&e) {
        m_entries.push_back(move(e));
    }

    void write(std::string const &fileName) const {
        std::ofstream file(fileName, std::ios::out);
        write(file);
    }

    void write(std::ostream &file) const {
        startFile(file);
        for(auto const & e: m_entries) {
            e.write(file, "\t");
        }
        endFile(file);
    }


private:
    void startFile(std::ostream &file) const {
        file << "<mdp>" << std::endl;
    }

    void endFile(std::ostream &file) const {
        file << "</mdp>" << std::endl;
    }
};
}

#endif // XMLWRITTER_H
