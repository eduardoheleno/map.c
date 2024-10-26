#include <stdio.h>
#include <raylib.h>

#include "xml/xml_parser.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

const char *error;

int main(void)
{
    NodeList node_list;
    node_list.size = 0;
    node_list.nodes = NULL;

    const char *file_name = "data.xml";
    if (!parse_xml(file_name, &node_list)) {
        error = "Couldn't load xml file";
        goto main_error;
    }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Screen");
    SetTargetFPS(60);

    Vector2 *base_mouse_pos = NULL;

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);

            for (size_t i = 0; i < node_list.size; i++) {
                Node *n = node_list.nodes[i];

                DrawPixel(n->x, n->y, WHITE);
            }

            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (base_mouse_pos == NULL) {
                    base_mouse_pos = malloc(sizeof(Vector2));
                    *base_mouse_pos = GetMousePosition();
                }

                Vector2 cur_mouse_pos = GetMousePosition();
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
