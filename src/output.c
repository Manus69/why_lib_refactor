#include "declarations.h"

Int FileCreate(const char* file_name)
{
    int file;

    if ((file = open(file_name, O_CREAT | O_WRONLY)) < 0)
        return WHY_ERROR;
    
    return WHY_OK;
}