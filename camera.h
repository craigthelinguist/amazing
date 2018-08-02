

#ifndef CAMERA_H
#define CAMERA_H

#include <stdint.h>

typedef struct Camera *Camera;

Camera make_camera(float x, float y);

void free_camera(Camera cam);

void pan_camera(Camera cam, float dx, float dy);

#endif

