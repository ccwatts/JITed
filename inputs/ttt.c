#include "generic.h"
#include <stdio.h>
#include <stdlib.h>

char* s = "Hello world";

struct union_t ut(struct union_t args) {
    struct union_t rt = {NULL, &s};
    return rt;
}