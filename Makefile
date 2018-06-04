BIN = bin
BUILD = build
INCLUDE = include
SRC = src
EXE = main

#~ CXX = g++
CXX = gcc
#~ SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib
#~ SDL_INCLUDE = -I/usr/include/SDL2
LOCAL_INCLUDE = -I$(INCLUDE)
#~ CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE) $(LOCAL_INCLUDE)
CXXFLAGS = -Wall $(LOCAL_INCLUDE)
#~ LDFLAGS = $(SDL_LIB)

all: $(EXE)

$(EXE): $(BUILD)/main.o $(BUILD)/muscatupa.o $(BUILD)/blur.o $(BUILD)/gifsave_wrapper.o $(BUILD)/colormap.o
	mkdir -p $(BIN)
	$(CXX) $^ $(LDFLAGS) -o $(BIN)/$@
	
$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
run:
	$(BIN)/$(EXE)

clean:
	rm -r $(BUILD) && rm -r $(BIN)
