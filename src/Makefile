CFLAGS = #-Wall -Werror -Wextra
CFLAGS += $(shell sdl2-config --cflags)
LDFLAGS = -lm
LDFLAGS += $(shell sdl2-config --libs)

BUILD_DIR = ../build

all: main circle

main: main.o
	gcc $(CFLAGS) -o $(BUILD_DIR)/$@ $^ $(LDFLAGS)

circle: circle.o
	gcc $(CFLAGS) -o $(BUILD_DIR)/$@ $^ $(LDFLAGS)
