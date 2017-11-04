

#ifndef CAMERA_H
#define CAMERA_H

#include <stdint.h>

typedef struct Camera *Camera;

Camera make_camera(int32_t x, int32_t y);

void free_camera(Camera cam);

void pan_camera(Camera cam, int32_t dx, int32_t dy);

#endif

