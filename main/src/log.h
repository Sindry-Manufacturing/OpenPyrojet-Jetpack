#ifndef OPENPYROJET_LOG_H
#define OPENPYROJET_LOG_H

#include <stdio.h>

#define LOG_DEBUG(...) fprintf(stdout, __VA_ARGS__)
#define LOG_ERROR(...) fprintf(stderr, __VA_ARGS__)

#endif // OPENPYROJET_LOG_H
