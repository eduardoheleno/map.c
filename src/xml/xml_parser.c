#include "xml/xml_parser.h"
#include "libxml/tree.h"

Node *parse_node(xmlNode *xml_node)
{
    Node *n = malloc(sizeof(Node));
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

    n->x = (int)(fractional_x * 2000);
    n->y = (int)(fractional_y * 2000);

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
