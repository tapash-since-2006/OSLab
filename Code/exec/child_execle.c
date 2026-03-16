#include <stdio.h>
#include <stdlib.h>

int main() {
    char *env_var = getenv("MYVAR");
    printf("child_execle running, MYVAR=%s\n", env_var);
    return 7;
}
