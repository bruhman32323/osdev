#include <std/util.h>

void* malloc(size_t amount) {
	char variable[amount];
	return &variable;
}

static size_t rand_next = 1;

int rand(void) {
    rand_next = rand_next * 1103515245 + 12345;
    return (size_t)(rand_next/65536) % 32768;
}

void srand(size_t seed) {
    rand_next = seed;
}

char* itoa(int res) {
    int size = 0;
    int t = res;

    while(t / 10 != 0) {
        t = t/10;
        size++;
    }
    static char ret[64];
    size++;
    ret[size] = '\0';
    t = res;
    int i = size - 1;
    while(i >= 0) {
        ret[i] = (t % 10) + '0';
        t = t/10;
        i--;
    }

    return ret;
}