#include "tracedebug.h"


typedef  std::shared_ptr<TraceDebug> ptr_TraceDebug;

bool TraceDebug::patternRequired = false;
bool TraceDebug::timesRequied    = false;
ptr_TraceDebug TraceDebug::s_instance{};

ptr_TraceDebug getTraceInstance(void) {
        if(!TraceDebug::s_instance)
            TraceDebug::s_instance = std::make_shared<TraceDebug>(TraceDebug());
        return TraceDebug::s_instance;
}
