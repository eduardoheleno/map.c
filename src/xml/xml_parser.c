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

bool parse_xml(const char *file_name, NodeList *node_list)
{
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    doc = xmlReadFile(file_name, NULL, 0);
    root_element = xmlDocGetRootElement(doc);

    xmlNode *cur_element = root_element->children;
    while (cur_element != NULL) {
        if (cur_element->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(cur_element->name, (const xmlChar*)"node")) {
                Node *n = parse_node(cur_element);
                node_list->nodes = realloc(node_list->nodes, sizeof(Node*) * ++node_list->size);
                node_list->nodes[node_list->size - 1] = n;
            }
        }

        cur_element = cur_element->next;
    }

    return true;
}
