#include "b_search.h"

NodeL *insert_ordered_node(NodeL **last_inserted_node, NodeL **root_nodel, Node *node,
    size_t *nodel_size)
{
    if ((*last_inserted_node) == NULL) {
        (*last_inserted_node) = malloc(sizeof(NodeL));
        (*last_inserted_node)->prev = NULL;
        (*last_inserted_node)->next = NULL;
        (*last_inserted_node)->node = node;

        *root_nodel = *last_inserted_node;

        (*nodel_size)++;
        return *last_inserted_node;
    }

    NodeL *cur_node = (*last_inserted_node);
    NodeL *nl = malloc(sizeof(NodeL));
    nl->node = node;

    if (node->id > cur_node->node->id) {
        while (cur_node->next != NULL) {
            if (cur_node->next->node->id > node->id) {
                nl->next = cur_node->next;
                nl->prev = cur_node;

                cur_node->next->prev = nl;
                cur_node->next = nl;

                (*nodel_size)++;
                return nl;
            }

            cur_node = cur_node->next;
        }

        nl->next = NULL;
        nl->prev = cur_node;

        cur_node->next = nl;

        (*nodel_size)++;
        return nl;
    }

    while (cur_node->prev != NULL) {
        if (cur_node->prev->node->id < node->id) {
            nl->next = cur_node;
            nl->prev = cur_node->prev;

            cur_node->prev->next = nl;
            cur_node->prev = nl;
        }

        cur_node = cur_node->prev;
    }

    nl->prev = NULL;
    nl->next = cur_node;

    cur_node->prev = nl;

    (*nodel_size)++;
    return nl;
}

Node *node_binary_search(NodeList *node_list, unsigned long ref)
{
    int low_point = 0;
    int high_point = (int)node_list->size - 1;
    int mid_point;

    while (low_point <= high_point) {
        mid_point = (low_point + high_point) / 2;

        if (ref == node_list->nodes[mid_point]->id) {
            return node_list->nodes[mid_point];
        }

        if (ref > node_list->nodes[mid_point]->id) {
            low_point = mid_point + 1;
        } else {
            high_point = mid_point - 1;
        }
    }

    return NULL;
}
