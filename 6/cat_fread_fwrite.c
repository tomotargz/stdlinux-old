#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static void do_cat(FILE *f, const char *s);
static FILE *get_file(const char *path);
static void die(const char *s);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        do_cat(stdin, "stdin");
    }
    for (int i = 1; i < argc; ++i) {
        do_cat(get_file(argv[i]), argv[i]);
    }
    exit(0);
}

static FILE *get_file(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) die(path);
    return f;
}

#define BUFFER_SIZE 2048

static void do_cat(FILE *f, const char *s) {
    unsigned char buf[BUFFER_SIZE];
    for (;;) {
        size_t n = fread(buf, 1, sizeof buf, f);
        if (feof(f) && ferror(f)) die(s);
        errno = 0;
        if (fwrite(buf, 1, n, stdout) < 0 && errno) die(s);
        if (feof(f)) break;
    }
    if (fclose(f) == EOF) die(s);
}

static void die(const char *s) {
    perror(s);
    exit(1);
}
