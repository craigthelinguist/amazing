CC=gcc

STD=-std=c11

# Warning flags.
WARNINGS= -Wall
LINKING= -IC:/MinGW/include/SDL2 -LC:/MinGW/lib -lSDL2 -lSDL2_image

SRC_MAIN=main.c graph.c gui.c drawing.c game.c

# Compilation rules.
amazing.exe:
	$(CC) $(SRC_MAIN) $(STD) $(WARNINGS) $(FLAGS) $(LINKING) $^ -o $@
