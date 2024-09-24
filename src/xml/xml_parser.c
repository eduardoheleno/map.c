#include "xml/xml_parser.h"

void skip_line(FILE *fptr)
{
    while (fgetc(fptr) != '\n');
}

char *remove_beg_spaces(const char *line_buffer, size_t line_size)
{
    int space_qtd = 0;
    while (line_buffer[space_qtd] == ' ') space_qtd++;

    char *line = malloc(sizeof(char) * line_size - space_qtd);
    cpystr_nullc(line, line_buffer + space_qtd, line_size - space_qtd);

    return line;
}

char *read_line(FILE *fptr)
{
    fpos_t pos_1, pos_2;
    fgetpos(fptr, &pos_1);

    char c = fgetc(fptr);
    size_t line_size = 0;

    while (c != '\n') {
        line_size++;
        c = fgetc(fptr);
    }

    fgetpos(fptr, &pos_2);
    fsetpos(fptr, &pos_1);

    char line_buffer[line_size + 1];
    fgets(line_buffer, line_size + 1, fptr);

    char *line = remove_beg_spaces(line_buffer, line_size);
    fsetpos(fptr, &pos_2);

    return line;
}

Node *parse_node(const char *line, int line_accumulator)
{
    Node *node = malloc(sizeof(Node));

    for (;;) {
        size_t attr_size = 0;
        int i = line_accumulator;
        while (line[i] != '=') {
            i++;
            attr_size++;
        }

        char attr_name_buffer[attr_size];
        cpystr_nullc(attr_name_buffer, line + line_accumulator, attr_size);

        printf("name: %s\n", attr_name_buffer);

        line_accumulator += attr_size + 1;

        if (line[line_accumulator] != '"') exit(1);
        line_accumulator++;

        size_t attr_value_size = 0;
        int j = line_accumulator;
        while (line[j] != '"') {
            j++;
            attr_value_size++;
        }

        char attr_value_buffer[attr_value_size];
        cpystr_nullc(attr_value_buffer, line + line_accumulator, attr_value_size);

        line_accumulator += attr_value_size + 1;

        if (strcmp(attr_name_buffer, "id") == 0) {
            int id = atoi(attr_value_buffer);
            node->id = id;
        } else if (strcmp(attr_name_buffer, "uid") == 0) {
            int uid = atoi(attr_value_buffer);
            node->uid = uid;
        } else if (strcmp(attr_name_buffer, "lat") == 0) {
            float lat = atof(attr_value_buffer);
            node->lat = lat;
        } else if (strcmp(attr_name_buffer, "lon") == 0) {
            float lon = atof(attr_value_buffer);
            node->lon = lon;
        }

        if (line[line_accumulator] == '/') {
            break;
        } else if (line[line_accumulator == '>']) {
            // TODO
        }

        line_accumulator++;
    }

    return node;
}

void *parse_type(const char *line, enum XmlType *type)
{
    int line_accumulator = 0;
    size_t ident_size = 0;

    while (line[ident_size] != ' ') ident_size++;

    char ident_buffer[ident_size];
    memcpy(ident_buffer, line, ident_size);

    line_accumulator += ident_size + 1;

    if (strcmp(ident_buffer, "<node") == 0) {
        *type = NODE;
        return (void*) parse_node(line, line_accumulator);
    } else if (strcmp(ident_buffer, "<tag") == 0) {
        // TODO
    }

    return NULL;
}

void *parse_line(const char *line, enum XmlType *type)
{
    return parse_type(line, type);
}

bool parse_xml(const char *file_name)
{
    FILE *fptr = fopen(file_name, "r");
    if (fptr == NULL) {
        return false;
    }

    skip_line(fptr);
    skip_line(fptr);
    skip_line(fptr);

    enum XmlType type;

    Node *nptr;

    char *line = read_line(fptr);
    void *parsed_line = parse_line(line, &type);

    switch (type) {
        case NODE:
            nptr = parsed_line;
        case TAG:
            // TODO
            break;
    }

    printf("uid: %i\n", nptr->uid);

    fclose(fptr);

    return true;
}
