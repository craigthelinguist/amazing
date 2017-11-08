
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <SDL_image.h>

#include "imagelib.h"

/// Maximum length of an asset name.
#define STRING_SIZE 32

struct Entry {
	struct Image image;
	char fname[STRING_SIZE];
};

struct ImageLib {
	int capacity;
	int size;
	char *base_path;
	struct Entry *entries;
};

/// The actual image library at runtime.
static struct ImageLib lib;

/// The error code for when library calls go wrong.
ImageLibErr IMAGE_LIB_ERR;

char streq(const char *s1, const char *s2) {
	return !strcmp(s1, s2);
}

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

bool directory_exists(char *dirpath) {
	struct stat info;
	if (stat(dirpath, &info) != 0) {
		return false;
	} else if (info.st_mode & S_IFDIR) {
		return true;
	} else {
		return false;
	}
}

bool file_exists(char *fpath) {
	struct stat info;
	if (stat(fpath, &info) != 0) {
		return false;
	} else if (info.st_mode & S_IFREG) {
		return true;
	} else {
		return false;
	}
}

bool imagelib_init(char *asset_directory, const int maxSize) {
	if (!directory_exists(asset_directory)) {
		IMAGE_LIB_ERR = DIRECTORY_NOT_FOUND;
		return false;
	}
	lib.entries = calloc(sizeof(struct Entry), maxSize);
	lib.capacity = maxSize;
	lib.size = 0;
	if (asset_directory != NULL) {
		int asset_dir_len = strlen(asset_directory);
		lib.base_path = malloc(asset_dir_len + 2); // +1 for null terminator, +1 for separator
		strcpy(lib.base_path, asset_directory);
		lib.base_path[asset_dir_len] = fpath_separator();
		lib.base_path[asset_dir_len + 1] = '\0';
	}
	return true;
}

void imagelib_free() {
	for (int i = 0; i < lib.capacity || lib.entries + i != NULL; i++) {
		SDL_DestroyTexture(lib.entries[i].image.texture);
	}
	free(lib.base_path);
	free(lib.entries);
}

bool imagelib_load(char *fname, SDL_Renderer *renderer) {

    // Check if the image is already loaded.
    if (imagelib_get(fname) != NULL) {
		return true;
	}

	if (lib.size == lib.capacity) {
		IMAGE_LIB_ERR = LIBRARY_FULL;
		return false;
	}

	int file_name_len = strlen(fname);
	if (file_name_len > STRING_SIZE - 1) {
		IMAGE_LIB_ERR = FILE_NAME_TOO_LONG;
		return false;
	}

	// Resolve the file name.
	int base_path_len = strlen(lib.base_path);
	char fpath[base_path_len + file_name_len + 1];
	strcpy(fpath, lib.base_path);
	strcat(fpath, fname);

	if (!file_exists(fpath)) {
		IMAGE_LIB_ERR = FILE_NOT_FOUND;
		return false;
	}

	SDL_Surface *surface = IMG_Load(fpath);
	if (surface == NULL) {
		IMAGE_LIB_ERR = SDL_ERROR;
		return false;
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);	
	if (!texture) {
		IMAGE_LIB_ERR = SDL_ERROR;
		free(surface);
		return false;
	}
	
	// Store the texture and image entries.
	lib.entries[lib.size].image.wd = surface->w;
	lib.entries[lib.size].image.ht = surface->h;
	lib.entries[lib.size].image.texture = texture;

	// Remember the filepath associated with this texture.
	strcpy(lib.entries[lib.size].fname, fname);

	// Clean up, increment size.
	lib.size++;
	SDL_FreeSurface(surface);
	return true;
	
}

struct Image *imagelib_get(char *fname) {
	for (int i = 0; i < lib.size; i++) {
		if (!strcmp(fname, lib.entries[i].fname))
			return &(lib.entries[i].image);
	}
	IMAGE_LIB_ERR = NO_SUCH_IMAGE;
	return NULL;
}
