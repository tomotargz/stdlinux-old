#include <stdio.h>
#include <stdlib.h>
#define _GNU_SOURCE
#include <getopt.h>

static void do_cat(FILE *f, const char *s);
static void die(const char *s);

int show_tab = 0;
int show_line_break = 0;

static const struct option longopts[] = {
    {"show-tab", no_argument, &show_tab, 1},
    {"show-line-break", no_argument, &show_line_break, 1},
    {"help", no_argument, NULL, 'h'},
    {0, 0, 0, 0},
};

int main(int argc, char *argv[]) {
    int opt;

    while ((opt = getopt_long(argc, argv, "tlh", longopts, NULL)) != -1) {
        switch (opt) {
            case 't':
                show_tab = 1;
                break;
            case 'l':
                show_line_break = 1;
                break;
            case 'h':
                fprintf(stdout, "Usage: %s [-t] [-l] [-h] [FILE ...]\n",
                        argv[0]);
                break;
            case '?':
                fprintf(stderr, "Usage: %s [-t] [-l] [-h] [FILE ...]\n",
                        argv[0]);
                break;
        }
    }

    if (optind == argc) {
        do_cat(stdin, "stdin");
    } else {
        for (int i = optind; i < argc; ++i) {
            FILE *f = fopen(argv[i], "r");
            if (!f) die(argv[i]);
            do_cat(f, argv[i]);
            if (fclose(f) == EOF) die(argv[i]);
        }
    }
    return 0;
}

static void do_cat(FILE *f, const char *s) {
    int c;
    while ((c = fgetc(f)) != EOF) {
        if (c == '\t' && show_tab) {
            if (putchar('\\') < 0) die(NULL);
            if (putchar('t') < 0) die(NULL);
        } else if (c == '\n' && show_line_break) {
            if (putchar('$') < 0) die(NULL);
            if (putchar('\n') < 0) die(NULL);
        } else {
            if (putchar(c) < 0) die(NULL);
        }
    }
}

static void die(const char *s) {
    perror(s);
    exit(1);
}
