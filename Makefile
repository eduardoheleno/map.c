CFLAGS := -Wall -Wextra -Wconversion -pedantic -std=gnu11 -I./include -I./external/raylib/src
LIBS := external/raylib/src/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11 `xml2-config --cflags --libs`
COMPILER := gcc
SRC_FILES := src/main.c src/b_search.c src/utils.c src/xml/xml_parser.c
all: map run

map: $(SRC_FILES)
	$(COMPILER) $(CFLAGS) -g -o map $(SRC_FILES) $(LIBS)

raylib:
	cd external/raylib/src/ && make PLATFORM=PLATFORM_DESKTOP

run:
	./map

clean:
	rm -f map
