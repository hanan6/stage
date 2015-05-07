#include "mdp.h" // mdp
#include "omp.h" // omp_set_num_threads
#include "fastareader.h" // FastaReader
#include "database.h"    // Database
#include "help.h"        // Help
#include "utility.h"
#include "tracedebug.h"
#include <iostream> // cout, endl
#include <getopt.h> // struct option, getopt_long
#include <string>   // string
#include <unordered_set>

#include "pattern.h"


using namespace std;
using namespace mdp;
using namespace mdp::database;



int main(int argc, char **argv) {

    string input{};
    string output{};
    string convName{};
    string generateName{};
    string patternName{};
    size_t minSlack{};
    size_t maxSlack = std::numeric_limits<size_t>::max();
    size_t nthread = omp_get_max_threads();
    bool   convMode = false;
    bool   generateMode = false;
    bool   patternMode = false;
    bool   patternSeqMode = false;
    bool   patternTimeMode = false;
//    bool   patternTimeSeqMode = false;
//    bool   verbose{};
    unordered_set<string>  dummies{}; // name of the dummy classes

    int c{};
    while(true) {
        int option_index = 0;
        static struct option long_options[] = {
                {"input",      required_argument, 0, 'i' }, // name of the input file
                {"output",     required_argument, 0, 'o' }, // name of the ouput file
                {"minslack",   required_argument, 0, 'm' }, // minimal slack
                {"maxslack",   required_argument, 0, 'M' }, // maximal slack
                {"verbose",    no_argument,       0, 'v' }, //
                {"dummy",      required_argument, 0, 'D' }, //
                {"help",       no_argument,       0, 'h' }, // force the type of the input file
                {"patterns",   no_argument,       0, 'P' }, //
                {"times",      no_argument,       0, 'T' }, //
                {"conv",       required_argument, 0, 'C' }, //
                {"nthread",   required_argument, 0, 'n' }, //
                {"generate",   required_argument, 0, 'g' }, //
                {"patterns-slack",   required_argument, 0, 'p' }, // patterns evolution wit the slack
                {"patterns-sequence",   required_argument, 0, 's' }, // patterns evolution wit the sequence
                {"patterns-time",   required_argument, 0, 't' }, // patterns evolution wit the sequence
                {0,            0,                 0,  0 }
        };

        c = getopt_long(argc, argv, "i:o:m:M:vhD:PTC:g:p:s:t:S:",
                        long_options, &option_index);
        if (c == -1)
                break;

        switch (c) {
        case 0:
            Help::print(cout);
            exit(0);
            break;
        case 'i':
            input = optarg;
            break;
        case 'o':
            output = optarg;
            break;
        case 'm':
            minSlack = atoi(optarg);
            break;
        case 'M':
            maxSlack = atoi(optarg);
            break;
        case 'D':
            dummies = split(optarg, ':');
            break;
        case 'v':
            //    verbose = true;
            break;
        case 'P':
            TraceDebug::patternRequired = true;
            break;
        case 'T':
            TraceDebug::timesRequied = true;
            break;
        case 'C':
            convMode = true;
            convName = optarg;
            break;
        case 'g':
            generateMode = true;
            generateName = optarg;
            break;
        case 'p':
            patternMode = true;
            patternName = optarg;
            break;
        case 's':
            patternSeqMode = true;
            patternName = optarg;
            break;
        case 'n':
            nthread = max(1, atoi(optarg));
            nthread = min(nthread, (size_t)omp_get_max_threads());
            break;
        case 't':
            patternTimeMode = true;
            patternName = optarg;
            break;
        case 'h':
            Help::print(cout);
            exit(0);
            break;
        case '?':
                break;
        default:
                printf("?? getopt returned character code 0%o ??\n", c);
        }
    }



    if(!(generateMode || convMode || patternMode || patternSeqMode || patternTimeMode) && input.empty()) {
        cout << "Aucun fichier d'entrée saisi." << endl;
        exit(0);
    } else if(!(generateMode || convMode|| patternMode || patternSeqMode || patternTimeMode) && output.empty()) {
        cout << "Aucun fichier de sortie saisi." << endl;
        exit(0);
    } else if(!(generateMode || convMode|| patternMode || patternSeqMode || patternTimeMode) && input == output) {
        cout << "Les fichiers en entrée et en sortie doivetn être distincts." << endl;
        exit(0);
    } else if(!(generateMode || convMode|| patternMode || patternSeqMode || patternTimeMode) && minSlack < 0) {
        cout << "minSlack doit être supérieur ou égal à 0." << endl;
        exit(0);
    } else if(!(generateMode || convMode|| patternMode || patternSeqMode || patternTimeMode) && maxSlack < 0) {
        cout << "maxSlack doit être supérieur ou égal à 0." << endl;
        exit(0);
    } else if(!(generateMode || convMode|| patternMode || patternSeqMode || patternTimeMode) && maxSlack < minSlack) {
        cout << "maxSlack doit être supérieur ou égal à minSlack." << endl;
        exit(0);
    }

    auto db = FastaReader::read(input, dummies);
    //db->saveDB();;
    //exit(0);
    if(convMode) {
        db->save(convName);
    } else if(generateMode) {
        MDP mdp(db, minSlack, maxSlack);
        mdp.runEmbeddings(generateName);
    } else if(patternMode) {
        MDP mdp(db, minSlack, maxSlack);
        mdp.runPatterns(patternName);
    } else if(patternSeqMode) {
        MDP mdp(db, minSlack, maxSlack);
        mdp.runPatternsSeq(patternName);
    }  else if(patternTimeMode) {
        MDP mdp(db, minSlack, maxSlack);
        mdp.runPatternsTime(patternName);
    } else {
        MDP mdp(db, minSlack, maxSlack);
        mdp.run(output);
    }


    return 0;
}

