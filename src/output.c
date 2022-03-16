#include "declarations.h"

bool FileExists(const char* file_name)
{
    struct stat stats;

    return stat(file_name, &stats) == 0;
}

Int FileCreate(const char* file_name)
{
    int file;

    if ((file = open(file_name, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR)) < 0)
        return WHY_ERROR;

    return file;
}

Int FileTruncate(const char* file_name)
{
    int file;

    if ((file = open(file_name, O_TRUNC) < 0))
        return WHY_ERROR;
    
    return file;
}

Int FileWriteStringInto(Int file, const char* string)
{
    Uint length;

    length = strlen(string);
    if (write(file, string, length) < 0)
        return WHY_ERROR;
    
    return WHY_OK;
}

Int FileWriteStringIntoN(Int file, const char* string)
{
    Uint    length;
    char*   new_line;

    length = strlen(string);
    new_line = "\n";

    if (write(file, string, length) < 0)
        return WHY_ERROR;

    if (write(file, new_line, 1) < 0)
        return WHY_ERROR;

    return WHY_OK;
}

Int FileWriteStringsInto(Int file, const Deck* strings)
{
    char*   string;
    Uint    n;
    Uint    n_items;

    n = 0;
    n_items = DeckNItems(strings);
    while (n < n_items)
    {
        string = *(char **)DeckPointAt(strings, n);
        FileWriteStringIntoN(file, string);

        ++ n;
    }

    return WHY_OK;
}