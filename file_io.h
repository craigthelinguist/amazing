
#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdbool.h>

/// Return the character which separates the names in a filepath. E.g. '/' or '\'
/// This is platform-specific.
char fpath_separator(void);

/// Check if the directory exists.
bool directory_exists(const char *dirpath);

/// Check if the file exists.
bool file_exists(const char *fpath);

#endif
