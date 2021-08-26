#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

static void do_tail(FILE* f, long n);

static const long DEFAULT_N_LINES = 10;

int main(int argc, char* argv[]) {
    long n = DEFAULT_N_LINES;
    int opt;
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
            case 'n':
                n = atol(optarg);
                if (n < 0) exit(0);
                break;
            case '?':
                fprintf(stderr, "Usage: %s [-n nlines] [FILE ...]", argv[0]);
                exit(1);
                break;
        }
    }
    if (optind == argc) {
        do_tail(stdin, n);
    } else {
        for (int i = optind; i < argc; ++i) {
            FILE* f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }
            do_tail(f, n);
            if (fclose(f) == EOF) {
                perror(argv[i]);
                exit(1);
            }
        }
    }
    exit(0);
}

static void do_tail(FILE* f, long n) {
    long l = 0;
    int c;
    int prev = '\n';
    while ((c = getc(f)) != EOF) {
        if (c == '\n') {
            ++l;
        }
        prev = c;
    }
    if (prev != '\n') ++l;
    rewind(f);
    while ((c = getc(f)) != EOF) {
        if (l <= n) {
            if (putchar(c) < 0) exit(1);
        }
        if (c == '\n') {
            --l;
        }
    }
}
