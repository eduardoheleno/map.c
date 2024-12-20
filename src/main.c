#include <stdio.h>
#include <string.h>
#include <raylib.h>

#include "xml/xml_parser.h"
#include "utils.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

const char *error;
int map_zoom = 1;

void test(Node *n)
{
    if (n->tags_size > 0) {
        printf("%s: %s\n", n->tags[0]->k, n->tags[0]->v);
    }
}

int main(void)
{
    NodeList node_list;
    WayList way_list;

    node_list.size = 0;
    way_list.size = 0;
    node_list.nodes = NULL;
    way_list.ways = NULL;

    const char *file_name = "data.xml";
    if (!parse_xml(file_name, &node_list, &way_list)) {
        error = "Couldn't load xml file";
        goto main_error;
    }

    normalize_nodes(&node_list);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Map");
    SetTargetFPS(60);

    Vector2 *base_mouse_pos = NULL;

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);

            float mouse_wheel = GetMouseWheelMove();
            set_map_zoom(&map_zoom, mouse_wheel);

            Vector2 cur_mouse_pos = GetMousePosition();
            Node *node_closer_to_mouse = NULL;

            for (size_t i = 0; i < node_list.size; i++) {
                Node *n = node_list.nodes[i];
                set_node_zoom(n, map_zoom, mouse_wheel);

                DrawPixel((int)n->x, (int)n->y, WHITE);

                if (node_closer_to_mouse == NULL) {
                    node_closer_to_mouse = n;
                } else {
                    double distance = sqrt(
                        pow((int)cur_mouse_pos.x - n->x, 2) +
                        pow((int)cur_mouse_pos.y - n->y, 2)
                    );
                    double best_distance = sqrt(
                        pow((int)cur_mouse_pos.x - node_closer_to_mouse->x, 2) +
                        pow((int)cur_mouse_pos.y - node_closer_to_mouse->y, 2)
                    );

                    if (distance < best_distance && n->tags_size > 0) {
                        node_closer_to_mouse = n;
                    }
                }
            }

            for (size_t i = 0; i < way_list.size; i++) {
                Way *w = way_list.ways[i];
                if (w->nodes_size > 0) {
                    Node *cur_node = w->nodes[0];
                    for (size_t j = 1; j < w->nodes_size; j++) {
                        Node *dest_node = w->nodes[j];

                        DrawLine((int)cur_node->x, (int)cur_node->y,
                                 (int)dest_node->x, (int)dest_node->y, WHITE);
                        cur_node = dest_node;
                    }
                }
            }

            DrawCircle((int)node_closer_to_mouse->x, (int)node_closer_to_mouse->y, 5,
                       BLUE);
            test(node_closer_to_mouse);

            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (base_mouse_pos == NULL) {
                    base_mouse_pos = malloc(sizeof(Vector2));
                    *base_mouse_pos = GetMousePosition();
                }

                for (size_t i = 0; i < node_list.size; i++) {
                    node_list.nodes[i]->x += (int)(cur_mouse_pos.x - base_mouse_pos->x);
                    node_list.nodes[i]->y += (int)(cur_mouse_pos.y - base_mouse_pos->y);
                }

                *base_mouse_pos = cur_mouse_pos;
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                free(base_mouse_pos);
                base_mouse_pos = NULL;
            }

        EndDrawing();
    }

    return 0;

main_error:
    perror(error);
    return 1;
}
