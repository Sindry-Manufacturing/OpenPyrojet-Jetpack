#ifndef OPENPYROJET_TIME_H
#define OPENPYROJET_TIME_H

#ifdef IDF_TARGET
#include <sys/time.h>
#include <time.h>
extern int usleep(useconds_t us);
#else // macOS / Linux
#include <zconf.h>
#endif

#endif // OPENPYROJET_TIME_H
