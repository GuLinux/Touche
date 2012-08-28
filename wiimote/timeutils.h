#ifndef TIMEUTILS_H
#define TIMEUTILS_H
#include "time.h"
#include <Qt>
class TimeUtils
{
public:
    TimeUtils();
    static qint64 nsecDifference(timespec before, timespec after);
    static qint64 nsecDifference(timespec *before, timespec *after);
};

#endif // TIMEUTILS_H
