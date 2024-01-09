#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <time.h>
#include <sys/time.h>

long long getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    long long timestamp = tv.tv_sec * 1000000 + tv.tv_usec;
    return timestamp;
}

void generateUUID(char* uuid, int number) {
    const char* characters = "0123456789abcdef";
    int i, rnd;

    // 生成随机数种子
    srand((getCurrentTime()+number)); 
    // 又不是不能用！

    // 生成32位随机数
    for (i = 0; i < 32; i++) {
        if (i == 8 || i == 12 || i == 16 || i == 20) {
            uuid[i] = '-';
        }
        else {
            rnd = rand() % 16;
            uuid[i] = characters[rnd];
        }
    }

    uuid[32] = '\0';
}

int main(int argc, char* argv[]) {
    int number = 1;
    if (argc == 2)
        number = atoi(argv[1]);
    while (number--)
    {
        char uuid[32];

        generateUUID(uuid,number);
        printf("%s\n", uuid);

    }
    
    return 0;
}