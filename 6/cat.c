#include <stdio.h>
#include <stdlib.h>

static void die(const char *s);

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        FILE *f = fopen(argv[i], "r");
        if (!f) {
            die(argv[i]);
        }
        int c;
        while ((c = fgetc(f)) != EOF) {
            if (putchar(c) < 0) die(NULL);
        }
        fclose(f);
    }
    exit(0);
}

static void die(const char *s) {
    perror(s);
    exit(1);
}
