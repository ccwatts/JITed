extern int f(void);

int multiple_args(int* args) {
    int a = f();
    return args[0] + args[1] + args[2] + args[3] + args[4];
}
