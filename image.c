
#include "image0.h"

SDL_Rect extract_img(image_sheet *sheet, int index) {
    int tiles_per_row = sheet->img.wd / sheet->img_size;
    int row = index / tiles_per_row;
    int col = index % tiles_per_row;
    return extract_img_rowcol(sheet, row, col);
}

SDL_Rect extract_img_rowcol(image_sheet *sheet, int row, int col) {
    int x = col * sheet->img_size;
    int y = row * sheet->img_size;
    return (SDL_Rect) { x, y, sheet->img_size, sheet->img_size };
}
