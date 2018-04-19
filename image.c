
#include "image0.h"

SDL_Rect extract_img(image_sheet *sheet, int index) {
    int tiles_per_row = sheet->img.wd / sheet->img_size;
    int row = index / tiles_per_row;
    int col = index % tiles_per_row;
    return extract_img_rowcol(sheet, row, col);
}

void extract_rowcol_from_index(image_sheet *sheet, int index, int *row, int *col) {

    int number_of_images = (sheet->img.wd / sheet->img_size) * (sheet->img.ht / sheet->img_size);
    if (index < 0 || index >= number_of_images) {
        fprintf(stderr, "Index %d is out of bounds on image of dimension %d x %d\n", index, sheet->img.wd, sheet->img.ht);
    }

    int tiles_per_row = sheet->img.wd / sheet->img_size;
    *row = index / tiles_per_row;
    *col = index % tiles_per_row;
}

SDL_Rect extract_img_rowcol(image_sheet *sheet, int row, int col) {
    int x = col * sheet->img_size;
    int y = row * sheet->img_size;
    return (SDL_Rect) { x, y, sheet->img_size, sheet->img_size };
}

image_sheet make_image_sheet(image image, uint16_t img_size) {

    if (image.ht % img_size != 0 || image.wd % img_size != 0) {
        fprintf(stderr, "ERROR: making image sheet. img_size is %d, but image dimensions are %d x %d\n",
                img_size, image.wd, image.ht);
        exit(12409812);
    }

    image_sheet sheet;
    sheet.img = image;
    sheet.img_size = img_size;
    return sheet;

}
