PROJECT = muscatupa
SRC = src/
CFLAGS = -Wall
OUT = 0.gif

all:
	gcc $(CFLAGS) -o $(SRC)$(PROJECT) $(SRC)$(PROJECT).c $(SRC)utils.c $(SRC)blur.c

run:
	./$(SRC)$(PROJECT)
	eog $(OUT)
