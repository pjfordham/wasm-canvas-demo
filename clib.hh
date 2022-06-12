#ifndef CLIB_FILE_H
#define CLIB_FILE_H

extern "C" inline char* strcpy(char* destination, const char* source)
{
    char *ptr = destination;
    while (*source != '\0')
    {
        *destination++ = *source++;
    }
    *destination = '\0';
    return ptr;
}

#endif
