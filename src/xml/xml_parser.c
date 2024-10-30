#include "xml/xml_parser.h"
#include "b_search.h"

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

Node *parse_nd(xmlNode *xml_node, NodeList *node_list)
{
    unsigned long ref = strtoul((char*)xmlGetProp(xml_node, (const xmlChar*)"ref"), NULL, 0);
    return node_binary_search(node_list, ref);
}

Way *parse_way(xmlNode *xml_node, NodeList *node_list)
{
    Way *w = malloc(sizeof(Way));
    w->tags = NULL;
    w->nodes = NULL;
    w->tags_size = 0;
    w->nodes_size = 0;

    char *id = (char*)xmlGetProp(xml_node, (const xmlChar*)"id");
    char *uid = (char*)xmlGetProp(xml_node, (const xmlChar*)"uid");

    w->id = strtoul(id, NULL, 0);
    w->uid = strtoul(uid, NULL, 0);

    if (xml_node->children != NULL) {
        xmlNode *cur_children = xml_node->children;
        while (cur_children != NULL) {
            if (!xmlStrcmp(cur_children->name, (const xmlChar*)"nd")) {
                Node *n = parse_nd(cur_children, node_list);
                if (n != NULL) {
                    w->nodes = realloc(w->nodes, sizeof(Node) * ++w->nodes_size);
                    w->nodes[w->nodes_size - 1] = n;
                }
            } else if (!xmlStrcmp(cur_children->name, (const xmlChar*)"tag")) {
                Tag *t = parse_tag(cur_children);
                w->tags = realloc(w->tags, sizeof(Tag*) * ++w->tags_size);
                w->tags[w->tags_size - 1] = t;
            }

            cur_children = cur_children->next;
        }
    }

    return w;
}

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

    n->x = (int)(E_RADIUS * cos(n->lat) * cos(n->lon));
    n->y = (int)(E_RADIUS * cos(n->lat) * sin(n->lon));

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

NodeList *init_node_list(xmlNode *root_element)
{
    xmlNode *cur_element = root_element->children;

    NodeL *last_inserted_node = NULL;
    NodeL *root_nodel = NULL;
    size_t nodel_size = 0;

    while (cur_element != NULL) {
        if (cur_element->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(cur_element->name, (const xmlChar*)"node")) {
                Node *n = parse_node(cur_element);
                last_inserted_node = insert_ordered_node(&last_inserted_node, &root_nodel, n, &nodel_size);
            }
        }

        cur_element = cur_element->next;
    }

    NodeList *node_list = malloc(sizeof(NodeList));
    node_list->nodes = malloc(sizeof(Node*) * nodel_size);
    node_list->size = nodel_size;

    for (size_t i = 0; i < node_list->size; i++) {
        Node *n = root_nodel->node;
        node_list->nodes[i] = n;

        NodeL *cur_nodel = root_nodel;
        root_nodel = root_nodel->next;

        // is this make sense?
        free(cur_nodel);
        cur_nodel = NULL;
    }

    return node_list;
}

WayList *init_way_list(xmlNode *root_element, NodeList *node_list)
{
    xmlNode *cur_element = root_element->children;
    WayList *way_list = malloc(sizeof(WayList));

    way_list->ways = NULL;
    way_list->size = 0;

    while (cur_element != NULL) {
        if (cur_element->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(cur_element->name, (const xmlChar*)"way")) {
                Way *w = parse_way(cur_element, node_list);
                way_list->ways = realloc(way_list->ways, sizeof(Way*) * ++way_list->size);
                way_list->ways[way_list->size - 1] = w;
            }
        }

        cur_element = cur_element->next;
    }

    return way_list;
}

bool parse_xml(
    const char *file_name, NodeList *node_list, WayList *way_list
    )
{
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    doc = xmlReadFile(file_name, NULL, 0);
    root_element = xmlDocGetRootElement(doc);

    *node_list = *init_node_list(root_element);
    *way_list = *init_way_list(root_element, node_list);

    return true;
}
