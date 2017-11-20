
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <SDL_Image.h>

#include "file_io.h"
#include "graphical_constants.h"
#include "imagelib0.h"

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

bool imagelib_init(const char *asset_directory, const int maxSize) {
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

/// Load an image from the given absolute filepath. The relative file name is also needed, as the image will be keyed on
/// this in the library.
bool imagelib_load_absolute(const char *fpath, const char *fname, SDL_Renderer *renderer) {

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

bool imagelib_load(const char *fname, SDL_Renderer *renderer) {

    // Check if the image is already loaded.
    if (imagelib_get(fname) != NULL) {
		return true;
	}

	// Check if there is space in the library.
	if (lib.size == lib.capacity) {
		IMAGE_LIB_ERR = LIBRARY_FULL;
		return false;
	}

	// Check if the name of the file isn't too long.
	int file_name_len = strlen(fname);
	if (file_name_len > STRING_SIZE - 1) {
		IMAGE_LIB_ERR = FILE_NAME_TOO_LONG;
		return false;
	}

    // Append the file extension and get the absolute path.
    char *fname_with_extension = fname_append(fname, ".png");
	char *fpath = resolve_fname(fname_with_extension);

    // Now load the image.
	bool result = imagelib_load_absolute(fpath, fname, renderer);
	free(fname_with_extension);
    free(fpath);
    return result;

}

struct Image *imagelib_get(const char *fname) {
	for (int i = 0; i < lib.size; i++) {
		if (!strcmp(fname, lib.entries[i].fname))
			return &(lib.entries[i].image);
	}
	IMAGE_LIB_ERR = NO_SUCH_IMAGE;
	return NULL;
}

char *fname_append(const char *string, const char *suffix) {
    int alloc_size = strlen(string) + strlen(suffix) + 1;
    char *result = calloc(sizeof(char), alloc_size);
    strcpy(result, string);
    strcat(result, suffix);
    return result;
}

/// Resolve the filename, turning it into an absolute filepath. This mallocs a string, which the caller must free. If
/// the file does not exist, then `NULL` is returned, and `IMAGE_LIB_ERR` is set to one of the following:
/// FILE_NAME_TOO_LONG, FILE_NOT_FOUND.
char *resolve_fname(const char *fname) {

    // Check the file name is not too long.
    int file_name_len = strlen(fname);
    if (file_name_len > STRING_SIZE - 1) {
        IMAGE_LIB_ERR = FILE_NAME_TOO_LONG;
        return false;
    }

    // Resolve the file name.
    int base_path_len = strlen(lib.base_path);
    char *fpath = malloc(sizeof(char) * (base_path_len + file_name_len + 1));
    strcpy(fpath, lib.base_path);
    strcat(fpath, fname);

    // Check the file path exists.
    if (!file_exists(fpath)) {
        IMAGE_LIB_ERR = FILE_NOT_FOUND;
        return false;
    }

    return fpath;

}

/// ===================================================================================================================
/// The following code is for loading sprite sheets.
/// ===================================================================================================================

cJSON *load_json(const char *fpath);
bool json_is_sprite(cJSON *root, Image *sprite_sheet);
Sprite sprite_from_json(cJSON *obj, Image *sprite_sheet);

/// Load a sprite with the given name. This will load the image into the library and return the data structure
/// containing information about the animations for the sprite. On failure, `NULL` is returned, and IMAGE_LIB_ERR is
/// set.
Sprite load_sprite(const char *fname, SDL_Renderer *renderer) {

    // If the associated image hasn't been loaded, then load it.
    if (!imagelib_load(fname, renderer))
        return NULL;
    Image *image = imagelib_get(fname);

    // Now open the metadata in the .json file.
    char *json_fname = fname_append(fname, ".json");
    char *json_fpath = resolve_fname(json_fname);
    cJSON *json = load_json_absolute(json_fpath);

    // Parse the JSON as a sprite sheet.
    Sprite sprite = NULL;
    if (json_is_sprite(json, image))
        sprite = sprite_from_json(json, image);
    free(json_fname);
    free(json_fpath);
    return sprite;

}

/// Check if the double `num` is also a natural number.
bool double_is_nat(double num) {
    double absolute = abs(num);
    return absolute == floor(absolute) && absolute >= 0;
}

cJSON *load_json_absolute(const char *fpath) {

	if (!file_exists(fpath)) {
		IMAGE_LIB_ERR = FILE_NOT_FOUND;
		return NULL;
	}

	// Compute the length of the file.
	FILE *fhandle = fopen(fpath, "rb");
	fseek(fhandle, 0, SEEK_END);
	size_t file_size = ftell(fhandle);
	rewind(fhandle);

	// Load contents into a string and parse as JSON.
	char *file_contents = malloc(sizeof(char) * file_size);
	fread(file_contents, sizeof(char), file_size, fhandle);
	fclose(fhandle);
	cJSON *data = cJSON_Parse(file_contents);

	// Clean up and return.
	free(file_contents);
	return data;

}

/// Load a JSON object from a .json file.
cJSON *load_json(const char *fname) {
	char *fpath = resolve_fname(fname);
	if (!fpath) return NULL;
	cJSON *obj = load_json_absolute(fpath);
	free(fpath);
	return obj;
}


/// Validate whether the given JSON object describes a sprite for the given sprite sheet. If the JSON object does not
/// correctly describe a sprite, then `false` is returned, and IMAGE_LIB_ERR is set.
bool json_is_sprite(cJSON *root, Image *sprite_sheet) {

	// If the root is `NULL`, then we did not load syntactically valid JSON.
	if (!root) {
		IMAGE_LIB_ERR = BAD_JSON;
		return false;
	}

	// The JSON must be an object.
	if (!cJSON_IsObject(root)) {
		IMAGE_LIB_ERR = SS_ROOT_NOT_OBJECT;
		return false;
	}

    // Now look at each animation in the sprite sheet.
	cJSON *array;
	cJSON_ArrayForEach(array, root) {

		// Offsets must be specified in an array.
		if (!cJSON_IsArray(array)) {
			IMAGE_LIB_ERR = SS_MUST_SPECIFY_OFFSETS_IN_ARRAY;
			return false;
		}

		// Animation name not allowed to be too long.
		if (strlen(array->string) > MAX_ANIMATION_NAME_LEN - 1) {
			IMAGE_LIB_ERR = NAME_TOO_LONG;
			return false;
		}

        // Now look at each frame in this animation.
		cJSON *offset;
		int num_offsets = 0;
		cJSON_ArrayForEach(offset, array) {
			num_offsets++;

			// Ensure the offset is a value.
			if (!cJSON_IsNumber(offset) || !double_is_nat(offset->valuedouble)) {
                IMAGE_LIB_ERR = OFFSETS_MUST_BE_NATURAL_NUMBERS;
                return false;
            }

            // Ensure the offset is on the image, and not too big.
            int offset_val = (int) floor(offset->valuedouble);
            if (num_offsets % 2 && offset_val + SPRITE_WD > sprite_sheet->wd) {
                IMAGE_LIB_ERR = OFFSET_OUT_OF_BOUNDS;
                return false;
            } else if (offset_val + SPRITE_HT > sprite_sheet->ht) {
                IMAGE_LIB_ERR = OFFSET_OUT_OF_BOUNDS;
                return false;
            }

            // Error if the offset's too big (to prevent overflows).
            if (offset->valuedouble > pow(2, 16)) {
                IMAGE_LIB_ERR = OFFSET_TOO_BIG;
                return false;
            }

		}

        // There must be an even number of offsets (2 per frame).
        if (num_offsets % 2 != 0) {
            IMAGE_LIB_ERR = WRONG_NUMBER_OF_OFFSETS;
            return false;
        }

	}

    return true;

}




/// Create a new sprite from the given JSON object. No error-checking is done if the JSON object is invalid; you should
/// call `json_is_sprite` to ensure that it is valid before calling this. A fresh Sprite is allocated, which the caller
/// should free.
Sprite sprite_from_json(cJSON *obj, Image *sprite_sheet) {

    // Figure out how many animations in the sprite, and how many frames in all the animations.
    int num_animations = cJSON_GetArraySize(obj);
    int total_num_frames = 0;
    cJSON *anim;
    cJSON_ArrayForEach(anim, obj)
    {
        total_num_frames += cJSON_GetArraySize(anim);
    }

    // Now we know how much space to allocate for the Sprite.
    int sprite_size = sizeof(struct Sprite)
                      + sizeof(struct Animation) * num_animations
                      + sizeof(struct Offset) * total_num_frames;
    struct Sprite *sprite = calloc(1, sprite_size);

    // Initialise the fields in the sprite. The rest default to zero (because of `calloc`).
    sprite->num_animations = num_animations;
    // TODO: it doesn't like compiling without the cast. Why?
    sprite->current_animation = (struct Animation *) &sprite->animations; // the first animation

    // Loop over the animations in the sprite, initialising each one.
    int8_t *index_ptr = (int8_t *) & sprite->animations;
    cJSON_ArrayForEach(anim, obj) {

        // Initialise the fields in the array.
        struct Animation *animation = (struct Animation *) index_ptr;
        strcpy(animation->name, anim->string);
        animation->num_frames = cJSON_GetArraySize(anim);

        // Load the offsets into the array.
        cJSON *offset = anim->child;
        for (int j = 0; !offset;) {
            animation->offsets[j++].x = (int) floor(offset->valuedouble);
            offset = offset->next;
            animation->offsets[j++].y = (int) floor(offset->valuedouble);
            offset = offset->next;
        }

        // Figure out where the next Animation starts.
        index_ptr += sizeof(struct Animation) + sizeof(struct Offset) * animation->num_frames;

    }

    return sprite;

}







