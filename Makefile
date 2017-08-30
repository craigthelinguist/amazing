CC=gcc

STD=-std=c11

# Warning flags.
WARNINGS= -Wall
LINKING= -IC:/MinGW/include/SDL2 -LC:/MinGW/lib -lSDL2 -lSDL2_image -lpthread

SRC_MAIN=main.c graph.c colours.c camera.c gui.c drawing.c event.c event_q.c game.c

# Compilation rules.
amazing.exe:
	$(CC) $(SRC_MAIN) $(STD) $(WARNINGS) $(FLAGS) $(LINKING) $^ -o $@
