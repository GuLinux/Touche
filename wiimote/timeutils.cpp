#include "timeutils.h"
#include <QDebug>

TimeUtils::TimeUtils()
{
}

qint64 TimeUtils::nsecDifference(timespec before, timespec after)
{
    return nsecDifference(&before, &after);
}

qint64 TimeUtils::nsecDifference(timespec *before, timespec *after)
{
    qint64 elapsedSecs = after->tv_sec - before->tv_sec;
    qint64 nanoSeconds = after->tv_nsec - before->tv_nsec;
    return elapsedSecs * 1000000000 + nanoSeconds;
}
