#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static void do_wc(FILE *f, const char *s);
static FILE *get_file(const char *path);
static void die(const char *s);

unsigned long long line;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        do_wc(stdin, "stdin");
    } else {
        for (int i = 1; i < argc; ++i) {
            do_wc(get_file(argv[i]), argv[i]);
        }
    }
    printf("%lld\n", line);
    exit(0);
}

static FILE *get_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) die(path);
    return f;
}

static void do_wc(FILE *f, const char *s) {
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
    if (fclose(f) < 0) die(s);
}

static void die(const char *s) {
    perror(s);
    exit(1);
}
