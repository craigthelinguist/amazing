

SRC_FILES := main.c graph.c colours.c camera.c gui.c drawing.c game_state.c render.c keymap.c game_loop.c image.c imagelib.c sprite.c file_io.c tile_map.c utils.c

LIBS := -lSDL2 -lSDL2_image -lpthread -lcjson -lm

FLAGS := -D__unix__

HEADERS := -I/usr/include/SDL2

amazing:
	gcc $(SRC_FILES) $(FLAGS) -std=c11 $(LIBS) $(HEADERS) -o $@

clean:
	rm -f amazing

example:
	gcc example.c image.c imagelib.c file_io.c -lSDL2 -lSDL2_image -lcjson -lm -std=c11 -D__unix__ -I/usr/include/SDL2 -o $@

