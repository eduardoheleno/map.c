#include "utils.h"

void normalize_nodes(NodeList *node_list)
{
    if (node_list->size == 0) exit(1);

    int sub_x = node_list->nodes[0]->x;
    int sub_y = node_list->nodes[0]->y;

    for (size_t i = 0; i < node_list->size; i++) {
        node_list->nodes[i]->x -= sub_x;
        node_list->nodes[i]->y -= sub_y;

        /* int y = node_list->nodes[i]->y; */
        /* node_list->nodes[i]->y = -node_list->nodes[i]->x; */
        /* node_list->nodes[i]->x = y; */
    }
}
