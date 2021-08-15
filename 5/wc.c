#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static void do_wc(int fd, const char *s);
static int get_fd(const char *path);
static void die(const char *s);

unsigned long long line;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        do_wc(STDIN_FILENO, "stdin");
    }
    for (int i = 1; i < argc; ++i) {
        do_wc(get_fd(argv[i]), argv[i]);
    }
    printf("%lld\n", line);
    exit(0);
}

static int get_fd(const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) die(path);
    return fd;
}

#define BUFFER_SIZE 2048

static void do_wc(int fd, const char *s) {
    unsigned char buf[BUFFER_SIZE];
    for (;;) {
        int n = read(fd, buf, sizeof buf);
        if (n < 0) die(s);
        if (n == 0) break;
        for (int i = 0; i < n; ++i) {
            if (buf[i] == '\n') {
                ++line;
            }
        }
    }
    if (close(fd) < 0) die(s);
}

static void die(const char *s) {
    perror(s);
    exit(1);
}
