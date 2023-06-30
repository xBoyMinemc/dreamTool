#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// #define SERVER_IP "173.208.172.170"
// #define SERVER_PORT 19132

// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>
#include <netdb.h>
// #include <arpa/inet.h>

#define BUF_SIZE 1024

#define true	1
#define false	0

_Bool isIPAddress(const char* str) {
    int dotCount = 0;
    int len = strlen(str);
    int num = 0;

    for (int i = 0; i < len; i++) {
        if (str[i] == '.') {
            dotCount++;
            if (num < 0 || num > 255) {
                return false;
            }
            num = 0;
        } else if (str[i] >= '0' && str[i] <= '9') {
            num = num * 10 + (str[i] - '0');
        } else {
            return false;
        }
    }

    if (dotCount != 3 || num < 0 || num > 255) {
        return false;
    }

    return true;
}

char** resolveDomainName(const char* domainName, int* count) {
    struct addrinfo hints, *res, *p;
    int status;
    char ipstr[INET_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(domainName, NULL, &hints, &res)) != 0) {
        fprintf(stderr, "无法解析域名：%s\n", gai_strerror(status));
        return NULL;
    }

    int numResults = 0;
    for (p = res; p != NULL; p = p->ai_next) {
        numResults++;
    }

    char** results = (char**)malloc(numResults * sizeof(char*));
    int i = 0;
    for (p = res; p != NULL; p = p->ai_next) {
        struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
        void* addr = &(ipv4->sin_addr);

        // 将IPv4地址从二进制转换为点分十进制表示
        inet_ntop(AF_INET, addr, ipstr, sizeof ipstr);
        results[i] = strdup(ipstr);
        i++;
    }

    *count = numResults;
    freeaddrinfo(res);

    return results;
}

void hexBufferToString(const unsigned char* buffer, size_t length, char* str_buffer) {
    for (size_t i = 0; i < length; i++) {
        sprintf(&str_buffer[i*2], "%02X", buffer[i]);
    }
}

char* hexToUtf8(const char* hexString,int hexLength) {
    // printf("hexLength=> %d\n",hexLength);
    int utf8Length = hexLength / 2;
    char* utf8String = (char*) malloc(utf8Length + 1);
    // char cohexValue[3] = { 'C', '2', '\0' };
    char cohexValue[3] = { 'A', '7', '\0' };
    int codecimalValue = strtol(cohexValue, NULL, 16);
   
    int i, j;
    int colorCount = 0;
    for (i = 0, j = 0; i < hexLength; i += 2) {

        if(hexString[i] == 'C' && hexString[i + 1] == '2')
        {
            // printf("\n# %c %c \n",hexString[i+2],hexString[i+3]);
            i += 4;
            continue;
        }
        char hexValue[3] = { hexString[i], hexString[i + 1], '\0' };
        int decimalValue = strtol(hexValue, NULL, 16);
            utf8String[j++] = (char) decimalValue;
    }

    utf8String[j] = '\0';
    return utf8String;
}

int main(int argc, char *argv[]) {
    // printf("命令行参数的数量：%d\n", argc);
    // for (int i = 0; i < argc; i++) {
    //     printf("参数 %d: %s\n", i, argv[i]);
    // }
    char* SERVER_HOST;
    char* SERVER_IP;
    unsigned short SERVER_PORT;
    if(argc<=1){
        SERVER_HOST = "173.208.172.170";
        SERVER_PORT = 19132;
    }else if(argc==2){
        char* host;
        char* port;

        host = strtok(argv[1], ":");
        port = strtok(NULL, ":");
        if(!port){
            port = "19132";
        }
        if (host != NULL && port != NULL) {
            
            SERVER_HOST = host;
            char *endptr;
            unsigned short num = strtol(port, &endptr, 10);
            if (*endptr != '\0')
            {
            printf("Invalid number\n");
            }
            else
            {
                // printf("Converted number: %ld\n", num);
            }

            SERVER_PORT = num;
            // printf("IP: %s\n", host);
            // printf("Port: %s\n", port);
        }


    }else if(argc==3){

        char *endptr;
        unsigned short num = strtol(argv[2], &endptr, 10);
        if (*endptr != '\0')
        {
            // printf("Invalid number\n");
        }
        else
        {
            printf("Converted number: %ld\n", num);
        }
        
        // printf("参数 %d: %s\n", 0, argv[0]);
        // printf("参数 %d: %s\n", 1, argv[1]);
        // printf("参数 %d: %d\n", 2, num);

        SERVER_HOST = argv[1];
        SERVER_PORT = num; // argv[2];
    }
    //记录原始host
    // strncpy(SERVER_HOST, host, sizeof(host)-1);
    // SERVER_HOST[sizeof(SERVER_HOST)-1] = '\0';
            // printf("IPv4 SERVER_IP 地址：%s\n", SERVER_IP);


    if(!isIPAddress(SERVER_HOST)){
    int count;
        char** results = resolveDomainName(SERVER_HOST, &count);

        if (results != NULL) {
            // printf("解析结果：\n");
            for (int i = 0; i < count; i++) {
            // printf("IPv4地址：%s\n", results[i]);
            SERVER_IP = results[i];
            // printf("IPv4 SERVER_IP 地址：%s\n", SERVER_IP);
            // free(results[i]);
            // printf("IPv4 SERVER_IP 地址：%s\n", SERVER_IP);

            }

            free(results);
        }
    }else{
        SERVER_IP = SERVER_HOST;
    }


    int sock;
    struct sockaddr_in serverAddr, clientAddr;
    char sendBuf[] = {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc1, 0x1d, 0x00, 0xff, 0xff, 0x00, 0xfe, 0xfe, 0xfe, 0xfe, 0xfd, 0xfd, 0xfd, 0xfd, 0x12, 0x34, 0x56, 0x78, 0x9c, 0x18, 0x28, 0x7f, 0xe1, 0x64, 0x89, 0x8d};
    char recvBuf[BUF_SIZE];
    int sendLen, recvLen, addrLen;

    // 创建UDP套接字
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        // perror("socket creation failed");
        return 1;
    }
            // printf("IPv4 SERVER_HOST 地址：%s\n", SERVER_HOST);
            // printf("IPv4 SERVER_IP 地址：%s\n", SERVER_IP);
            // printf("IPv4 SERVER_PORT 地址：%d\n", SERVER_PORT);

    // 配置服务器地址信息
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 发送UDP数据包
    sendLen = sendto(sock, sendBuf, sizeof(sendBuf), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (sendLen < 0) {
        perror("sendto failed");
        close(sock);
        return 1;
    }
    // printf("UDP packet sent successfully!\n");

    // 设置超时时间为5秒
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    // printf("UDP packet timeout set %d s!\n", timeout.tv_sec);

    // 等待UDP回复
    addrLen = sizeof(clientAddr);
    recvLen = recvfrom(sock, recvBuf, BUF_SIZE, 0, (struct sockaddr*)&clientAddr, &addrLen);
    if (recvLen < 0) {
        perror("recvfrom failed");
        close(sock);
        return 1;
    }

    // 输出接收到的UDP回复
    // printf("UDP packet received from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    
    // 去除无意义的前35位
    recvLen -= 35;
    memmove(recvBuf, recvBuf + 35, recvLen);

    char str_buffer[recvLen * 2 + 1]; // 预留一个额外的字符用于存储字符串结束符'\0'
    hexBufferToString(recvBuf, recvLen, str_buffer);

    // printf("转换后的字符串: %s\n", str_buffer);

    char* motd = hexToUtf8(str_buffer,recvLen * 2 + 1);
    // char* motd
    // printf("转换后的字符串: %s\n", motd);
    char *token;
    
    // char *sign[] = {"服务器类型: ", "服务器名称: ", "协议=>", "版本=>", "在线人数: ", "最大人数: ","","地图名=> ", "默认模式: ", "默认模式: ", "IPV4端口: ",  "IPV6端口: ", "0=" };
    // char *sign[] = {"服务器类型: ", "服务器名称: ", "协议=>", "版本=>", "在线人数: ", "最大人数: ", "","地图名=> ", "默认模式: ", "默认模式: ", "IPV4端口: ",  "IPV6端口: ", "0=" };
    char *sign[] = {"O 服务器类型: ", "O 服务器名称: ", "O 协议=>", "O 版本=>", "O 在线人数: ", "O 最大人数: ", "O ","O 地图名=> ", "O 默认模式: ", "O 默认模式: ", "O IPV4端口: ",  "O IPV6端口: ", "O 0=" };
    char pos = 0; 

    // 使用 strtok 函数分割字符串
    token = strtok(motd, ";");

    // 循环打印分割后的子字符串
    int t = 0;
    while (token != NULL) {
        
        if(t!=6&&t!=12)
        printf("%s%s\n",sign[pos], token);
        ++t;
        ++pos;
        // printf("%s\n",  token);
        token = strtok(NULL, ";");
    }
    printf("O 解析地址: %s\n", inet_ntoa(clientAddr.sin_addr));
    // printf("解析地址 %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    
    // 关闭UDP套接字
    close(sock);

    return 0;
}
