#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static int buf[4];
static size_t buflen;

/* Reads from stdin until buf[index] can be accessed. */
static bool read_buf(size_t index) {
    while (!(index < buflen)) {
        buf[buflen] = getchar();
        if (buf[buflen] == EOF) {
            return false;
        }
        buflen++;
    }
    return buf[buflen - 1] != EOF;
}

static void write_buf(size_t n) {
    for (size_t i = 0; i < n; i++) {
        putchar(buf[i]);
    }
    buflen = 0;
}

static bool start_between(int min, int max) {
    return min <= buf[0] && buf[0] < max;
}

static bool is_cont(int index) {
    return 0x80 <= buf[index] && buf[index] < 0xC0;
}

static int code_point(int bits18, int bits12, int bits06, int bits00) {
    return   ((bits18 & 0x3F) << 18)
           | ((bits12 & 0x3F) << 12)
           | ((bits06 & 0x3F) <<  6)
           |  (bits00 & 0x3F);
}

int main(void) {
    while (read_buf(0) && !ferror(stdout)) {
        if (start_between(0x00, 0x80)) {
            write_buf(1);
        } else if (start_between(0xC2, 0xE0) && read_buf(1) && is_cont(1)
                   && code_point(0, 0, buf[0] & 0x1F, buf[1]) >= 0x0080) {
            write_buf(2);
        } else if (start_between(0xE0, 0xF0) && read_buf(2) && is_cont(1) && is_cont(2)
                   && code_point(0, buf[0] & 0x0F, buf[1], buf[2]) >= 0x0800) {
            write_buf(3);
        } else if (start_between(0xF0, 0xF8) && read_buf(3) && is_cont(1) && is_cont(2) && is_cont(3)
                   && code_point(buf[0] & 0x07, buf[1], buf[2], buf[3]) >= 0x010000
                   && code_point(buf[0] & 0x07, buf[1], buf[2], buf[3]) < 0x110000) {
            write_buf(4);
        } else {
            putchar(0xC0 + ((buf[0] >> 6) & 0x1F));
            putchar(0x80 + ((buf[0] >> 0) & 0x3F));
            buflen--;
            buf[0] = buf[1];
            buf[1] = buf[2];
            buf[2] = buf[3];
            buf[3] = 0;
        }
    }

    return ferror(stdin) || ferror(stdout) ? EXIT_FAILURE : EXIT_SUCCESS;
}
