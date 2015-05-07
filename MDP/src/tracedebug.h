#ifndef TRACEDEBUG_H
#define TRACEDEBUG_H

#include <memory>
#include <sstream>
#include <fstream>

class TraceDebug
{
public:
    typedef  std::shared_ptr<TraceDebug> ptr_TraceDebug;
private:
    //static bool           s_isCreate;
    static ptr_TraceDebug s_instance;



    std::ofstream os_times;    // current file used
    std::ofstream os_patterns; // current file used
private:
    TraceDebug() {

    }

public:
    ~TraceDebug() {

    }

    TraceDebug(TraceDebug const &td)

    {}

    void setTimes(std::string const &fileBaseName) {
        if(os_times.is_open())
            os_times.close();
        os_times.open(fileBaseName);
    }

    void setPatterns(std::string const &fileBaseName) {
        if(os_patterns.is_open())
            os_patterns.close();
        os_patterns.open(fileBaseName);
    }

    void writeTime(size_t slack, size_t time1, size_t time2) {
        os_times << slack << "\t" << time1 << "\t" << time2 << std::endl;
    }

    void writePatterns(size_t sequences, size_t patterns, size_t time) {
        os_patterns << sequences << "\t" << patterns << "\t" << time << std::endl;
    }

public:
    friend ptr_TraceDebug getTraceInstance(void);


    static bool patternRequired;
    static bool timesRequied;
};


typedef  std::shared_ptr<TraceDebug> ptr_TraceDebug;
ptr_TraceDebug getTraceInstance(void);

#endif // TRACEDEBUG_H
