CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
LIBS = -lm -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

SRC_DIR = src
_SRCFILES = main.c scene.c title.c game.c solver.c resources.c utils.c

SRCFILES = $(patsubst %,$(SRC_DIR)/%,$(_SRCFILES))

TARGET = quantum

quantum: $(SRCFILES)
	$(CC) -o $(TARGET) $(SRCFILES) $(LIBS) $(CFLAGS)

solve_main: src/solve_main.c src/solver.c
	$(CC) -o solve_main $^ $(LIBS) $(CFLAGS)

clean:
	rm $(TARGET)
