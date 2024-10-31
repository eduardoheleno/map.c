#include "utils.h"

void normalize_nodes(NodeList *node_list)
{
    if (node_list->size == 0) exit(1);

    double sub_x = node_list->nodes[0]->x;
    double sub_y = node_list->nodes[0]->y;

    for (size_t i = 0; i < node_list->size; i++) {
        node_list->nodes[i]->x -= sub_x;
        node_list->nodes[i]->y -= sub_y;

        node_list->nodes[i]->y = -node_list->nodes[i]->y;
    }
}

void set_map_zoom(int *map_zoom, float mouse_wheel)
{
    if (mouse_wheel != 0) {
        if (mouse_wheel > 0) {
            *map_zoom += 1;
        } else if (mouse_wheel < 0 && *map_zoom > 1) {
            *map_zoom -= 1;
        }
    }
}

void set_node_zoom(Node *node, int map_zoom, float mouse_wheel)
{
    if (node->zoom != (int)map_zoom) {
        if (mouse_wheel > 0) {
            node->x *= (int)map_zoom;
            node->y *= (int)map_zoom;
        } else {
            node->x /= (int)map_zoom + 1;
            node->y /= (int)map_zoom + 1;
        }
        node->zoom = (int)map_zoom;
    }
}
