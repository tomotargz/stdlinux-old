#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

static void do_grep(regex_t *pat, FILE *src);
int opt_ignore_case = 0;
int opt_invert = 0;

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "iv")) != -1) {
        switch (opt) {
            case 'i':
                opt_ignore_case = 1;
                break;
            case 'v':
                opt_invert = 1;
                break;
        }
    }
    if (argc == optind) {
        fputs("no pattern\n", stderr);
        exit(1);
    }
    regex_t pat;
    int mode = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;
    if (opt_ignore_case) mode |= REG_ICASE;
    int err = regcomp(&pat, argv[optind], mode);
    if (err != 0) {
        char buf[1024];
        regerror(err, &pat, buf, sizeof buf);
        puts(buf);
        exit(1);
    }
    if (argc == optind + 1) {
        do_grep(&pat, stdin);
    } else {
        for (int i = optind + 1; i < argc; i++) {
            FILE *f;
            f = fopen(argv[i], "r");
            if (!f) {
                perror(argv[i]);
                exit(1);
            }
            do_grep(&pat, f);
            fclose(f);
        }
    }
    regfree(&pat);
    exit(0);
}

static void do_grep(regex_t *pat, FILE *src) {
    char buf[4096];
    while (fgets(buf, sizeof buf, src)) {
        int match = (regexec(pat, buf, 0, NULL, 0) == 0);
        if (opt_invert) match = !match;
        if (match) {
            fputs(buf, stdout);
        }
    }
}
