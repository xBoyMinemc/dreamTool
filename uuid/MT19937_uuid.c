#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct {
    unsigned int mt[624];
    int index;
} MT19937;

long long getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    long long timestamp = tv.tv_sec * 1000000 + tv.tv_usec;
    return timestamp;
}


void initMT(MT19937* mt, unsigned int seed) {
    mt->mt[0] = seed;
    for (int i = 1; i < 624; i++) {
        mt->mt[i] = 1812433253 * (mt->mt[i - 1] ^ (mt->mt[i - 1] >> 30)) + i;
    }
    mt->index = 0;
}

unsigned int generateRandomNumber(MT19937* mt) {
    if (mt->index == 0) {
        for (int i = 0; i < 624; i++) {
            unsigned int y = (mt->mt[i] & 0x80000000) + (mt->mt[(i + 1) % 624] & 0x7fffffff);
            mt->mt[i] = mt->mt[(i + 397) % 624] ^ (y >> 1);
            if (y % 2 != 0) {
                mt->mt[i] ^= 0x9908b0df;
            }
        }
    }

    unsigned int y = mt->mt[mt->index];
    y ^= y >> 11;
    y ^= (y << 7) & 0x9d2c5680;
    y ^= (y << 15) & 0xefc60000;
    y ^= y >> 18;

    mt->index = (mt->index + 1) % 624;
    return y;
}

void generateUUID(char* uuid, MT19937* mt) {
    const char* characters = "0123456789abcdef";

 

    for (int i = 0; i < 36; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            uuid[i] = '-';
        }else if (i == 14) {
            uuid[i] = '4';
        }else if (i == 19) {
            uuid[i] = 'a';
        }
        else {
            unsigned int rnd = generateRandomNumber(mt) % 16;
            uuid[i] = characters[rnd];
        }
    }
    uuid[36] = '\0';
}
int main(int argc, char* argv[]) {
    int number = 1;
    if (argc == 2)
        number = atoi(argv[1]);
    while (number--){
        MT19937 mt;
        unsigned int seed = getCurrentTime();
        initMT(&mt, seed);

        char uuid[36];
        generateUUID(uuid, &mt);
        printf("%s\n", uuid);
    }

    return 0;
}
