
#include <sys\stat.h>

#include "file_io.h"

/// Return the character which separates the names in a filepath. E.g. '/' or '\'
/// This is platform-specific.
char fpath_separator() {
#ifdef __unix__
    return '/';
#elif defined(_WIN32) || defined(WIN32)
    return '\\';
#else
    fprintf(stderr, "Unknown platform");
    exit(100);
#endif
}

/// Check if the given file exists.
bool directory_exists (const char *fpath) {
    struct stat info;
    return !stat(fpath, &info) && (info.st_mode & S_IFDIR);
}

/// Check if the given file exists.
bool file_exists (const char *fpath) {
    struct stat info;
    return !stat(fpath, &info) && (info.st_mode & S_IFREG);
}
