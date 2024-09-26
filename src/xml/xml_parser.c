#include "xml/xml_parser.h"
#include "string_utils.h"

static FILE *file_ptr;

void skip_line()
{
    while (fgetc(file_ptr) != '\n');
}

char *remove_beg_spaces(const char *line_buffer, size_t line_size)
{
    int space_qtd = 0;
    while (line_buffer[space_qtd] == ' ') space_qtd++;

    char *line = malloc(sizeof(char) * line_size - space_qtd + 1);
    cpystr_nullc(line, line_buffer + space_qtd, line_size - space_qtd);

    return line;
}

char *read_line()
{
    fpos_t pos_1, pos_2;
    fgetpos(file_ptr, &pos_1);

    char c = fgetc(file_ptr);
    size_t line_size = 0;

    while (c != '\n') {
        line_size++;
        c = fgetc(file_ptr);
    }

    fgetpos(file_ptr, &pos_2);
    fsetpos(file_ptr, &pos_1);

    char line_buffer[line_size + 1];
    fgets(line_buffer, line_size + 1, file_ptr);

    char *line = remove_beg_spaces(line_buffer, line_size);
    fsetpos(file_ptr, &pos_2);

    return line;
}

char *extract_next_attr_name(const char *line, int *line_accumulator)
{
    size_t attr_name_size = 0;
    int i = *line_accumulator;
    while (line[i] != '=') {
        i++;
        attr_name_size++;
    }

    char *attr_name_buffer = malloc(sizeof(char) * attr_name_size + 1);
    cpystr_nullc(attr_name_buffer, line + *line_accumulator, attr_name_size);

    *line_accumulator += attr_name_size + 1;

    return attr_name_buffer;
}

char *extract_next_attr_value(const char *line, int *line_accumulator)
{
    if (line[*line_accumulator] != '"') exit(1);
    (*line_accumulator)++;

    size_t attr_value_size = 0;
    int i = *line_accumulator;
    while (line[i] != '"') {
        i++;
        attr_value_size++;
    }

    char *attr_value_buffer = malloc(sizeof(char) * attr_value_size + 1);
    cpystr_nullc(attr_value_buffer, line + *line_accumulator, attr_value_size);

    *line_accumulator += attr_value_size + 1;

    return attr_value_buffer;
}

void parse_tag(Node *node)
{
    for (;;) {
        char *tag_line = read_line();
        if (strcmp(tag_line, "</node>") == 0) break;

        int line_accumulator = 0;

        /* always expects '<tag' */
        char ident_buffer[4 + 1];
        cpystr_nullc(ident_buffer, tag_line, 4);
        if (strcmp(ident_buffer, "<tag") != 0) exit(1);

        line_accumulator += 4 + 1;

        Tag *tag = malloc(sizeof(Tag));

        for (;;) {
            char *attr_name_buffer = extract_next_attr_name(tag_line, &line_accumulator);
            char *attr_value_buffer = extract_next_attr_value(tag_line, &line_accumulator);

            if (strcmp(attr_name_buffer, "k") == 0) {
                tag->k = attr_value_buffer;
            } else if (strcmp(attr_name_buffer, "v") == 0) {
                tag->v = attr_value_buffer;
            }

            if (tag_line[line_accumulator] == '/') break;

            line_accumulator++;
        }

        node->tags = realloc(node->tags, sizeof(Tag*) * ++node->tags_size);
        node->tags[node->tags_size - 1] = tag;

        free(tag_line);
    }
}

Node *parse_node(const char *line, int line_accumulator)
{
    Node *node = malloc(sizeof(Node));

    for (;;) {
        char *attr_name_buffer = extract_next_attr_name(line, &line_accumulator);
        char *attr_value_buffer = extract_next_attr_value(line, &line_accumulator);

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

        free(attr_name_buffer);
        free(attr_value_buffer);

        if (line[line_accumulator] == '/') {
            break;
        } else if (line[line_accumulator] == '>') {
            parse_tag(node);
            break;
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

    char ident_buffer[ident_size + 1];
    cpystr_nullc(ident_buffer, line, ident_size);

    line_accumulator += ident_size + 1;

    if (strcmp(ident_buffer, "<node") == 0) {
        *type = NODE;
        return (void*) parse_node(line, line_accumulator);
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
    if (fptr == NULL) return false;

    file_ptr = fptr;

    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */
    /* skip_line(); */

    enum XmlType type;
    Node *nptr;

    char *line = read_line();
    void *parsed_line = parse_line(line, &type);

    switch (type) {
        case NODE:
            nptr = parsed_line;
            break;
    }

    /* printf("test: %s\n", nptr->tags[0]->v); */

    fclose(fptr);

    return true;
}
