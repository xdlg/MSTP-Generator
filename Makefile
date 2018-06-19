BIN = bin
BUILD = build
INCLUDE = include
SRC = src
EXE = main

CXX = g++
SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/include/SDL2
LOCAL_INCLUDE = -I$(INCLUDE)
CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE) $(LOCAL_INCLUDE)
LDFLAGS = $(SDL_LIB)

all: $(EXE)

$(EXE): $(BUILD)/main.o $(BUILD)/pattern.o $(BUILD)/blind_quarter.o \
		$(BUILD)/blur.o $(BUILD)/colormap.o $(BUILD)/symmetry.o
	mkdir -p $(BIN)
	$(CXX) $^ $(LDFLAGS) -o $(BIN)/$@
	
$(BUILD)/%.o: $(SRC)/%.cpp
	mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
run:
	$(BIN)/$(EXE) 320 240 dawn

clean:
	rm -r $(BUILD) && rm -r $(BIN)
