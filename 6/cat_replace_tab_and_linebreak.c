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
            if (c == '\t') {
                if (putchar('\\') < 0) die(NULL);
                if (putchar('t') < 0) die(NULL);
            } else if (c == '\n') {
                if (putchar('$') < 0) die(NULL);
                if (putchar('\n') < 0) die(NULL);
            } else {
                if (putchar(c) < 0) die(NULL);
            }
        }
        fclose(f);
    }
    exit(0);
}

static void die(const char *s) {
    perror(s);
    exit(1);
}
