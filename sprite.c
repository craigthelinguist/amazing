
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sprite0.h"

bool set_anim_name(Sprite sprite, const char *name, long long time_ms) {

    // Find the animation with the appropriate name (if it exists).
    struct Animation *anim = get_anim_by_name(sprite, name);
    if (!anim) {
        return false;
    }

    // Update sprite information, if it is a different animation.
    if (anim != sprite->current_animation) {
        sprite->frame_age = time_ms;
        sprite->current_animation = anim;
        sprite->current_frame = 0;
    }

    return true;

}

struct Animation *get_anim_by_index(Sprite sprite, int16_t index) {

    // Do some sanity checks.
    if (index < 0) {
        fprintf(stderr, "Failed trying to get animation at negative index %d\n", index);
        exit(700);
    } else if (index >= sprite->num_animations) {
        fprintf(stderr, "Failed trying to get animation at index which is too big: %d\n", index);
        exit(700);
    }

    // Now step through the array of animations.
    int8_t *needle = (int8_t *) &sprite->animations;
    for (int i = 0; i < index; i++) {
        struct Animation *anim = (struct Animation *) needle;
        needle += sizeof(struct Animation) + sizeof(struct Offset) * anim->num_frames;
    }
    return (struct Animation *) needle;

}

struct Animation *get_anim_by_name(Sprite sprite, const char *anim_name) {

    // Keep track of the current position in the array with `needle`.
    bool found_name = false;
    int8_t *needle = (int8_t *) &sprite->animations;

    // Step along the array.
    for (int i = 0; i < sprite->num_animations; i++) {
        struct Animation *anim = (struct Animation *) needle;

        if (!strcmp(anim->name, anim_name)) {
            found_name = true;
            break;
        }
        needle += sizeof(struct Animation) + sizeof(struct Offset) * anim->num_frames;
    }

    // If we found the name, return the position of the needle.
    return found_name ? (struct Animation *) needle : NULL;

}

void animate(Sprite sprite, long long time_ms) {
    const long long UPDATE_DELAY = sprite->current_animation->frame_delay;
    if (time_ms - sprite->frame_age > UPDATE_DELAY) {
        sprite->current_frame += 1;
        sprite->current_frame %= sprite->current_animation->num_frames;
        sprite->frame_age = time_ms;
    }
}

struct Offset get_current_frame(Sprite sprite) {
    int8_t current_frame = sprite->current_frame;
    Animation anim = sprite->current_animation;
    struct Offset *offset = anim->offsets;
    return anim->offsets[current_frame];
}
