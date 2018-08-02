

#ifndef POINT_H
#define POINT_H

#define DECL_PT(type) \
	struct pt_##type { \
        type a; \
        type b; \
    }; \
	\
	struct pt_##type make_pt_##type(type a, type b) { \
		return (struct pt_##type) { a, b }; \
	} \

struct pt_float {
    float x;
	float y;
};

struct Point_Int32 {
	int32_t x;
	int32_t y;
};

typedef struct Point_Int32 Point_Int32;

#endif