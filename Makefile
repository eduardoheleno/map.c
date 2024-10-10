CFLAGS := -Wall -Wextra -Wconversion -pedantic -std=gnu11 -I./include
LDFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
COMPILER := gcc
SRC_FILES := src/main.c src/xml/xml_parser.c src/string_utils.c

all: map run

map: $(SRC_FILES)
	$(COMPILER) -g -o map $(CFLAGS) $(SRC_FILES)

run:
	./map

clean:
	rm -f map
