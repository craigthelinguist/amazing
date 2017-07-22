
#include "camera.h"

#include "gui.h"
#include <stdlib.h>

Camera make_camera(int32_t x, int32_t y) {
	struct Camera *cam = malloc(sizeof(struct Camera));
	cam->centre_x = x;		// Starting position is the middle of the screen.
	cam->centre_y = y; 
	return cam;
}

void free_camera(Camera cam) {
	free(cam);
	cam = NULL;
}

void pan_camera(Camera cam, int32_t dx, int32_t dy) {
	cam->centre_x += dx;
	cam->centre_y += dy;
}
