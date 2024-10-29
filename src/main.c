#include <stdio.h>
#include <string.h>
#include <raylib.h>

#include "xml/xml_parser.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

const char *error;

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

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Screen");
    SetTargetFPS(60);

    Vector2 *base_mouse_pos = NULL;

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);

            Vector2 cur_mouse_pos = GetMousePosition();
            Node *node_closer_to_mouse = NULL;

            for (size_t i = 0; i < node_list.size; i++) {
                Node *n = node_list.nodes[i];
                DrawPixel(n->x, n->y, WHITE);

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

            DrawCircle(node_closer_to_mouse->x, node_closer_to_mouse->y, 5, BLUE);
            test(node_closer_to_mouse);

            // insert Node when preprocessing data
            /* for (size_t i = 0; i < way_list.size; i++) { */
            /*     Way *w = way_list.ways[i]; */
            /*     for (size_t j = 0; j < w->nodes_size; j++) { */

            /*     } */
            /* } */

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
