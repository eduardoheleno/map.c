#include <stdio.h>
#include "xml/xml_parser.h"

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

    return 0;

main_error:
    perror(error);
    return 1;
}
