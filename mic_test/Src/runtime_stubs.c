#include <sys/stat.h>
#include <stdint.h>

void _init(void) {}
void _fini(void) {}

int _close(int file) {
    (void)file;
    return -1;
}

int _fstat(int file, struct stat *st) {
    (void)file;
    if (st != 0) {
        st->st_mode = S_IFCHR;
    }
    return 0;
}

int _isatty(int file) {
    (void)file;
    return 1;
}

int _lseek(int file, int ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _read(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

void *_sbrk(int incr) {
    extern uint8_t _end;
    extern uint8_t _estack;
    static uint8_t *heap_end;
    uint8_t *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &_end;
    }

    prev_heap_end = heap_end;
    if ((heap_end + incr) > (&_estack)) {
        return (void *)-1;
    }

    heap_end += incr;
    return prev_heap_end;
}

int _write(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;
    return len;
}

void _exit(int status) {
    (void)status;
    while (1) {
    }
}

int _getpid(void) {
    return 1;
}

int _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    return -1;
}
