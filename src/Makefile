CC = gcc
SDL2_DIR = c:/prog/SDL2
CFLAGS = -std=c11 -Werror #-Wall -Wextra
CFLAGS += -I$(SDL2_DIR)/i686-w64-mingw32/include/SDL2
LDFLAGS = -lm
LDFLAGS += -L$(SDL2_DIR)/i686-w64-mingw32/lib -lmingw32 -lSDL2_test -lSDL2main -lSDL2

BUILD_DIR = ../build

all: main circle

main: main.o
	gcc $(CFLAGS) -o $(BUILD_DIR)/$@.exe $^ $(LDFLAGS)

circle: circle.o
	gcc $(CFLAGS) -o $(BUILD_DIR)/$@.exe $^ $(LDFLAGS)

testdraw2: testdraw2.o
	gcc $(CFLAGS) -o $(BUILD_DIR)/$@.exe $^ $(LDFLAGS)

%.o: %.c
	gcc -c $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o "$(BUILD_DIR)/*"

rebuild: clean all