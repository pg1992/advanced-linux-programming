#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(void)
{
    void *handle = dlopen("libtest.so", RTLD_LAZY);
    int (*test)() = dlsym(handle, "f");

    if (!test) {
        printf("%s\n", dlerror());
        exit(1);
    }
    printf("Value of f() = %d\n", (*test)());

    dlclose(handle);

    return 0;
}
