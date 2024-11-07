COMPILER := gcc

CFLAGS := -Wall -Wextra -Wconversion -pedantic -std=gnu11 -I./include -I./external/raylib/src
LIBS := external/raylib/src/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11 `./external/libxml2/build/bin/xml2-config --cflags --libs`
SRC_FILES := src/main.c src/b_search.c src/utils.c src/xml/xml_parser.c

PREFIX = $(shell pwd)

all: map run

map: $(SRC_FILES)
	$(COMPILER) $(CFLAGS) -g -o map $(SRC_FILES) $(LIBS)

raylib:
	cd external/raylib/src/ && \
	make PLATFORM=PLATFORM_DESKTOP

libxml2:
	cd external/libxml2/ && \
	./autogen.sh --prefix="$(PREFIX)/external/libxml2/build/" --enable-static --disable-shared && \
	make && \
	make install

run:
	./map

clean:
	rm -f map
