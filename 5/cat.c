#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static void do_cat(int fd, const char *s);
static int get_fd(const char *path);
static void die(const char *s);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        do_cat(STDIN_FILENO, "stdin");
    }
    for (int i = 1; i < argc; ++i) {
        do_cat(get_fd(argv[i]), argv[i]);
    }
    exit(0);
}

static int get_fd(const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) die(path);
    return fd;
}

#define BUFFER_SIZE 2048

static void do_cat(int fd, const char *s) {
    unsigned char buf[BUFFER_SIZE];
    for (;;) {
        int n = read(fd, buf, sizeof buf);
        if (n < 0) die(s);
        if (n == 0) break;
        if (write(STDOUT_FILENO, buf, n) < 0) die(s);
    }
    if (close(fd) < 0) die(s);
}

static void die(const char *s) {
    perror(s);
    exit(1);
}
