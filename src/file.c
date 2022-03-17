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

char* FileNameBase(const char* name)
{
    Int     index;
    char*   result;
    
    index = StringFindCLast(name, '/');
    if (index == NOT_FOUND)
        result = strdup(name);
    else result = strdup(name + index + 1);

    index = StringFindC(result, '.');
    if (index != NOT_FOUND)
        result[index] = 0;
    
    return result;
}