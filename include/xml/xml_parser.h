#ifndef XML_PARSER_H_
#define XML_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <string_utils.h>

enum XmlType {
    NODE,
    UNKNOWN
};

typedef struct Tag {
    char *k;
    char *v;
} Tag;

typedef struct Node {
    unsigned long id;
    unsigned long uid;
    double lat;
    double lon;

    size_t tags_size;
    Tag **tags;
} Node;

bool parse_xml(const char *file_name);

#endif // XML_PARSER_H_
