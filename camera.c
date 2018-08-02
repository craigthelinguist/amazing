
#include <stdlib.h>

#include "camera0.h"

Camera make_camera(float x, float y) {
	struct Camera *cam = malloc(sizeof(struct Camera));
	cam->centre_x = x;
	cam->centre_y = y; 
	return cam;
}

void free_camera(Camera cam) {
	free(cam);
	cam = NULL;
}

void pan_camera(Camera cam, float dx, float dy) {
	cam->centre_x += dx;
	cam->centre_y += dy;
}
