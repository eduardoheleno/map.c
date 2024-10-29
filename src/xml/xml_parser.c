#include "xml/xml_parser.h"
#include "libxml/tree.h"

Tag *parse_tag(xmlNode *xml_node)
{
    Tag *t = malloc(sizeof(Tag));
    char *k = (char*)xmlGetProp(xml_node, (const xmlChar*)"k");
    char *v = (char*)xmlGetProp(xml_node, (const xmlChar*)"v");

    t->k = k;
    t->v = v;

    return t;
}

/* Node *parse_nd(xmlNode *xml_node, NodeList *node_list) */
/* { */
/*     unsigned long ref = strtoul((char*)xmlGetProp(xml_node, (const xmlChar*)"ref"), NULL, 0); */
/*     for (size_t i = 0; i < node_list->size; i++) { */
/*         Node *n = node_list->nodes[i]; */
/*         if (n->id == ref) { */
/*             return n; */
/*         } */
/*     } */

/*     return NULL; */
/* } */

/* Way *parse_way(xmlNode *xml_node, NodeList *node_list) */
/* { */
/*     Way *w = malloc(sizeof(Way)); */
/*     w->tags = NULL; */
/*     w->nodes = NULL; */
/*     w->tags_size = 0; */
/*     w->nodes_size = 0; */

/*     char *id = (char*)xmlGetProp(xml_node, (const xmlChar*)"id"); */
/*     char *uid = (char*)xmlGetProp(xml_node, (const xmlChar*)"uid"); */

/*     w->id = strtoul(id, NULL, 0); */
/*     w->uid = strtoul(uid, NULL, 0); */

/*     if (xml_node->children != NULL) { */
/*         xmlNode *cur_children = xml_node->children; */
/*         while (cur_children != NULL) { */
/*             if (!xmlStrcmp(cur_children->name, (const xmlChar*)"nd")) { */
/*                 Node *n = parse_nd(cur_children, node_list); */
/*                 if (n != NULL) { */
/*                     w->nodes = realloc(w->nodes, sizeof(Node) * ++w->nodes_size); */
/*                     w->nodes[w->nodes_size - 1] = n; */
/*                 } */
/*             } else if (!xmlStrcmp(cur_children->name, (const xmlChar*)"tag")) { */
/*                 Tag *t = parse_tag(cur_children); */
/*                 w->tags = realloc(w->tags, sizeof(Tag*) * ++w->tags_size); */
/*                 w->tags[w->tags_size - 1] = t; */
/*             } */

/*             cur_children = cur_children->next; */
/*         } */
/*     } */

/*     return w; */
/* } */

Node *parse_node(xmlNode *xml_node)
{
    Node *n = malloc(sizeof(Node));
    n->tags = NULL;
    n->tags_size = 0;

    char *id = (char*)xmlGetProp(xml_node, (const xmlChar*)"id");
    char *uid = (char*)xmlGetProp(xml_node, (const xmlChar*)"uid");
    char *lat = (char*)xmlGetProp(xml_node, (const xmlChar*)"lat");
    char *lon = (char*)xmlGetProp(xml_node, (const xmlChar*)"lon");

    n->id = strtoul(id, NULL, 0);
    n->uid = strtoul(uid, NULL, 0);
    n->lat = atof(lat);
    n->lon = atof(lon);

    double lat_rad = n->lat * (M_PI / 180.0);
    double lon_rad = n->lon * (M_PI / 180.0);

    double x = E_RADIUS * lon_rad;
    double y = E_RADIUS * log(tan(M_PI / 4.0 + lat_rad / 2.0));

    double ux = x * -1;
    double uy = y * -1;

    double discart_integer;
    double fractional_x = modf(ux, &discart_integer);
    double fractional_y = modf(uy, &discart_integer);

    n->x = (int)(fractional_x * 5000);
    n->y = (int)(fractional_y * 5000);

    if (xml_node->children != NULL) {
        xmlNode *cur_children = xml_node->children;
        while (cur_children != NULL) {
            if (!xmlStrcmp(cur_children->name, (const xmlChar*)"tag")) {
                Tag *t = parse_tag(cur_children);
                n->tags = realloc(n->tags, sizeof(Tag*) * ++n->tags_size);
                n->tags[n->tags_size - 1] = t;
            }

            cur_children = cur_children->next;
        }
    }

    return n;
}

NodeL *insert_ordered_node(NodeL **last_inserted_node, NodeL **root_nodel, Node *node)
{
    if ((*last_inserted_node) == NULL) {
        (*last_inserted_node) = malloc(sizeof(NodeL));
        (*last_inserted_node)->prev = NULL;
        (*last_inserted_node)->next = NULL;
        (*last_inserted_node)->node = node;

        *root_nodel = *last_inserted_node;

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

                return nl;
            }

            cur_node = cur_node->next;
        }

        nl->next = NULL;
        nl->prev = cur_node;

        cur_node->next = nl;
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
    return nl;
}

bool parse_xml(
    const char *file_name, NodeList *node_list, WayList *way_list
    )
{
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    doc = xmlReadFile(file_name, NULL, 0);
    root_element = xmlDocGetRootElement(doc);

    xmlNode *cur_element = root_element->children;

    NodeL *last_inserted_node = NULL;
    NodeL *root_nodel = NULL;

    while (cur_element != NULL) {
        if (cur_element->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(cur_element->name, (const xmlChar*)"node")) {
                Node *n = parse_node(cur_element);
                node_list->nodes = realloc(node_list->nodes, sizeof(Node*) * ++node_list->size);
                node_list->nodes[node_list->size - 1] = n;

                last_inserted_node = insert_ordered_node(&last_inserted_node, &root_nodel, n);
            }
        }

        cur_element = cur_element->next;
    }

    return true;
}
