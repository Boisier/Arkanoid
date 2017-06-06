CC = gcc

#Remove # in LDFLAGS and CFLAGS to activate ASan

LIB     = -lSDL -lSDL_image -lSDL_mixer -lGLU -lGL -lm `pkg-config --libs freetype2` -lpthread
LDFLAGS = -fsanitize=address
CFLAGS = -g -O0 -Wall -std=c89 -pedantic -Werror `pkg-config --cflags freetype2` -g -fno-omit-frame-pointer -ggdb

APP_BIN = arkanopong

SRC_PATH = src
OBJ_PATH = obj
INC_PATH = -I include
BIN_PATH = bin
LIB_PATH = lib

SRC_FILES = $(shell find $(SRC_PATH) -type f -name '*.c')
OBJ_FILES = $(patsubst $(SRC_PATH)/%.c,$(OBJ_PATH)/%.o, $(SRC_FILES))

all: $(APP_BIN) clean display

$(APP_BIN): $(OBJ_FILES)
	@mkdir -p $(BIN_PATH)
	$(CC) -o $(BIN_PATH)/$(APP_BIN) $(OBJ_FILES) $(LDFLAGS) $(LIB) -Wall 

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p "$(@D)"
	$(CC) -c $< -o $@ $(CFLAGS) $(INC_PATH)

clean:
	rm -f $(OBJ_FILES)

display:
	clear
	@echo "--------------------------------------------------------------"
	@echo "                  Compile succeeded!                          "
	@echo "            To execute type: ./bin/$(APP_BIN)                 "
	@echo "--------------------------------------------------------------"