#include "utils.h"

// TODO: normalize on creation
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
            (*map_zoom)++;
        } else if (mouse_wheel < 0 && *map_zoom > 1) {
            (*map_zoom)--;
        }
    }
}

void set_node_zoom(Node *node, int map_zoom, float mouse_wheel)
{
    if (mouse_wheel > 0 && node->zoom != map_zoom) {
        node->x = (node->x / node->zoom) + node->x;
        node->y = (node->y / node->zoom) + node->y;

        node->zoom = map_zoom;
    } else if (mouse_wheel < 0 && node->zoom != map_zoom) {
        node->x = node->x - (node->x / node->zoom);
        node->y = node->y - (node->y / node->zoom);

        node->zoom = map_zoom;
    }
}
