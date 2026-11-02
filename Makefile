OBJ = a.out
SRC = main.c framebuffer.c game.c
CC  = gcc

$(OBJ) : $(SRC)
	$(CC) $(SRC) -lm

clean:
	rm a.out
