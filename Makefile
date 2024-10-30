CFLAGS := -Wall -Wextra -Wconversion -pedantic -std=gnu11 -I./include
LDFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 `xml2-config --cflags --libs`
COMPILER := gcc
SRC_FILES := src/main.c src/b_search.c src/utils.c src/xml/xml_parser.c
all: map run

map: $(SRC_FILES)
	$(COMPILER) -g -o map $(CFLAGS) $(SRC_FILES) $(LDFLAGS)

run:
	./map

clean:
	rm -f map
