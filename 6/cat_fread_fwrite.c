#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static void do_cat(FILE *f, const char *s);
static void die(const char *s);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        do_cat(stdin, "stdin");
    }
    for (int i = 1; i < argc; ++i) {
        FILE *f = fopen(argv[i], "r");
        if (!f) die(argv[i]);
        do_cat(f, argv[i]);
        if (fclose(f) == EOF) die(argv[i]);
    }
    exit(0);
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
}

static void die(const char *s) {
    perror(s);
    exit(1);
}
