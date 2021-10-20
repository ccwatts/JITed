#include "generic.h"
#include <stdio.h>

int func1() {
    return 3;
}

int func2() {
    return 4;
}

int add1(int x) {
    return x+1;
}

int add2(int x) {
    return x+2;
}

int mult3(int x) {
    return x*3;
}

int multiple_args(int* args) {
    return args[0] + args[1];
}

int ut(struct union_t args) {
    return func1();
}

int indirect2() {
    printf("Hi!");
    return 42;
}

int indirect1() {
    return indirect2();
}