#include <stdio.h>
#include <stdlib.h>

#include "util.h"

char* int_to_str(int i) {
    char* str = (char*)malloc(((int)(i/10)+1)*sizeof(char));
    sprintf(str, "%d", i);
    return str;
}