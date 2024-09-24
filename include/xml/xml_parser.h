#ifndef XML_PARSER_H_
#define XML_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <string_utils.h>

enum XmlType {
    NODE,
    TAG
};

typedef struct Node {
    int id;
    int uid;
    float lat;
    float lon;
} Node;

bool parse_xml(const char *file_name);

#endif // XML_PARSER_H_
