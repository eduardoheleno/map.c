#ifndef B_SEARCH_H_
#define B_SEARCH_H_

#include "xml/xml_parser.h"

NodeL *insert_ordered_node(NodeL **last_inserted_node, NodeL **root_nodel, Node *node,
    size_t *nodel_size);
Node *node_binary_search(NodeList *node_list, unsigned long ref);

#endif // B_SEARCH_H_
