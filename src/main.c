#include <stdio.h>
#include "xml/xml_parser.h"

const char *error;

int main(void)
{
    const char *file_name = "data.xml";
    if (!parse_xml(file_name)) {
        error = "Couldn't load xml file";
        goto main_error;
    }

    return 0;

main_error:
    perror(error);
    return 1;
}
