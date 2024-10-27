#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NVARS 3

int main() {
    char *var_names[NVARS] = {"MYVAR1", "MYVAR2", "MYVAR3"};
    char *var_values[NVARS] = {"lera1", "lera2", "lera3"};

    for (int i = 0; i < NVARS; i++) {
        if (setenv(var_names[i], var_values[i], 1) != 0) {
            perror("new variable");
            return EXIT_FAILURE;
        }
        printf("New environment variable: %s=%s\n", var_names[i], var_values[i]);
    }

    extern char **environ;
    printf("All environment variables:\n");
    for (char **env = environ; *env != 0; env++) {
        printf("%s\n", *env);
    }

    return EXIT_SUCCESS;
}