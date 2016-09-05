PROJECT = muscatupa
SRC = src/
CFLAGS = -Wall
OUT = 0.gif

all:
	gcc $(CFLAGS) -o $(SRC)$(PROJECT).out $(SRC)$(PROJECT).c $(SRC)utils.c $(SRC)blur.c $(SRC)colormap.c $(SRC)symmetry.c -lm

run:
	./$(SRC)$(PROJECT).out
	eog $(OUT)
	#ffmpeg -f gif -i 0.gif  0.mp4

