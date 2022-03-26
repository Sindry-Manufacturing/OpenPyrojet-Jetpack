#ifndef OPENPYROJET_FILE_H
#define OPENPYROJET_FILE_H

#include <stdio.h>
#include <stdbool.h>

bool file_exists(const char*);

/**
 * @return size in bytes
 */
long file_get_size(FILE*);

/**
 * @return bytes copied
 */
long file_copy(const char* fromPath, const char* toPath);

#endif //OPENPYROJET_FILE_H
