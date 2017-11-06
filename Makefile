CC=gcc

STD=-std=c11

# Warning flags.
WARNINGS= -Wall
LINKING= -IC:/MinGW/include/SDL2 -LC:/MinGW/lib -lSDL2 -lSDL2_image -lpthread

SRC_MAIN=main.c graph.c colours.c camera.c gui.c drawing.c game_state.c render.c keymap.c game_loop.c image.c imagelib.c

# Compilation rules.
amazing.exe:
	$(CC) $(SRC_MAIN) $(STD) $(WARNINGS) $(FLAGS) $(LINKING) $^ -o $@

clean:
	rm amazing.exe
