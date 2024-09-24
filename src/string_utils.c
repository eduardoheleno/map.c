#include "string_utils.h"

void cpystr_nullc(char *dest, const char *src, size_t size)
{
    memcpy(dest, src, size);
    dest[size] = '\0';
}
