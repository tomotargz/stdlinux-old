#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static void do_wc(FILE *f, const char *s);
static void die(const char *s);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        do_wc(stdin, "stdin");
    } else {
        for (int i = 1; i < argc; ++i) {
            FILE *f = fopen(argv[i], "r");
            if (!f) {
                die(argv[i]);
            }
            do_wc(f, argv[i]);
            if (fclose(f) == EOF) {
                die(argv[i]);
            }
        }
    }
    return 0;
}

static void do_wc(FILE *f, const char *s) {
    unsigned long line = 0;
    int prev = '\n';
    for (;;) {
        errno = 0;
        int c = fgetc(f);
        if (c == EOF) {
            if (errno != 0) die(s);
            if (prev != '\n') ++line;
            break;
        } else if (c == '\n') {
            ++line;
        }
        prev = c;
    }
    printf("%lu\n", line);
}

static void die(const char *s) {
    perror(s);
    exit(1);
}
