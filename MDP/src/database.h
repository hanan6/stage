#ifndef DATABASE_H
#define DATABASE_H

#include <vector>        // vector
#include <string>        // string
#include <unordered_map> // unordered_map
#include <unordered_set> // unordered_set
#include <set>           // set
#include <utility>       // pair

#include <iostream>
#include <sstream>
/*#include <QtSql/QSqlDatabase>
#include <QtSql/QMYSQLDriver>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtCore/QString>*/

namespace mdp { namespace database {
    typedef std::string      Sequence;
    typedef std::vector<Sequence> SequenceBase;
    typedef std::string           Name;
    typedef std::vector<Name>     NameBase;
    typedef std::unordered_map<Name, std::pair<NameBase, SequenceBase> >  Base;

    class Database {
    private:
        typedef std::unordered_map<Name, std::pair<NameBase, SequenceBase> >  Base;
        typedef std::set<std::string> SetString;
    public:
        typedef SetString::const_iterator const_iterator;
    private:
        SetString  classes;
        Base       base;
    public:
        Database(const Base& base)
        :  classes(), base(base)
        {
            for(typename Base::const_iterator it = base.begin(); it != base.end(); ++it)
                classes.insert(it->first);
        }
        template<class Container>
        void getBase(std::string n, Container& dest) const {
            dest.clear();
            dest.insert(dest.end(), base.at(n).second.begin(), base.at(n).second.end());
        }
        template<class Container>
        void getNames(std::string n, Container& dest) {
            dest.clear();
            dest.insert(dest.end(), base.at(n).first.begin(), base.at(n).first.end());
        }
        template<class Container>
        void getNamesExpect(std::string n, Container& dest) {
            dest.clear();
            for(typename Base::const_iterator it = base.begin(); it != base.end(); ++it) {
                if(it->first != n)
                    dest.insert(dest.end(), base.at(n).first.begin(), base.at(n).first.end());
            }
        }
        template<class Container>
        void getBasesExcept(std::string n, Container& dest) const {
            dest.clear();
            for(typename Base::const_iterator it = base.begin(); it != base.end(); ++it) {
                if(it->first != n)
                    dest.insert(dest.end(), it->second.second.begin(), it->second.second.end());
            }
        }

        void save(std::string const &fileName);

        const_iterator begin() const {
            return classes.begin();
        }
        const_iterator end() const {
            return classes.end();
        }
        size_t size() const {
            return classes.size();
        }
#if 0
    private:
        bool createConnection()
        {
            QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
            db.setHostName("localhost");
            db.setDatabaseName("test");
            db.setUserName("root");
            db.setPassword("coucou");
            if (!db.open()) {
                return false;
            }
            return true;
        }

    public:
        void saveDB(void) {
            bool co = createConnection();

            if(co) {
                std::cout << "Connecion réussie !" << std::endl;
            } else {
                std::cout << "Connecion echouée !" << std::endl;
            }

            for(typename Base::const_iterator it = base.begin(); it != base.end(); ++it) {
                for(size_t i = 0; i < (it->second).first.size(); ++i) {
                    std::ostringstream os_query;
                    os_query << "INSERT INTO LEAP_04_2015 (class_id, name, sequence) VALUES (";
                    os_query << "\"" << it->first << "\",";
                    os_query << "\"" << (it->second).first.at(i) << "\",";
                    os_query << "\"" << (it->second).second.at(i)<< "\"";
                    os_query << ")";
                    std::string str_query = os_query.str();
                    QString qstr_query(str_query.c_str());
                    std::cout << str_query << std::endl;
                    QSqlQuery query;
                    query.exec(qstr_query);
                }
            }
        }
#endif
    };
}}

#endif
