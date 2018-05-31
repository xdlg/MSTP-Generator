PROJECT = muscatupa
SRC = src/
CFLAGS = -Wall
OUT = 0.gif

all:
	gcc $(CFLAGS) -o $(SRC)$(PROJECT).out $(SRC)main.c $(SRC)muscatupa.c \
	$(SRC)utils.c $(SRC)blur.c $(SRC)gifsave_wrapper.c $(SRC)colormap.c -lm

run:
	./$(SRC)$(PROJECT).out
	#eog $(OUT)
	#ffmpeg -f gif -i 0.gif  0.mp4
