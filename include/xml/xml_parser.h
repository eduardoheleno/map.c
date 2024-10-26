#ifndef XML_PARSER_H_
#define XML_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>

#define E_RADIUS 6371.0

typedef struct Tag {
    char *k;
    char *v;
} Tag;

typedef struct Node {
    unsigned long id;
    unsigned long uid;
    double lat;
    double lon;

    int x;
    int y;

    Tag **tags;
    size_t tags_size;
} Node;

typedef struct NodeList {
    Node **nodes;
    size_t size;
} NodeList;

bool parse_xml(const char *file_name, NodeList *node_list);

#endif // XML_PARSER_H_
