#ifndef UTILS_H_
#define UTILS_H_

#include "xml/xml_parser.h"

void normalize_nodes(NodeList *node_list);
void set_map_zoom(int *map_zoom, float mouse_wheel);
void set_node_zoom(Node *node, int map_zoom, float mouse_wheel);

#endif // UTILS_H_
