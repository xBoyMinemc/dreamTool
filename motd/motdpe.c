//gcc motdpe.c -o motdpe.exe -lws2_32
//chcp 65001
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")
// play.easecation.net
#define SERVER_IP "173.208.172.170"
#define SERVER_PORT 19132
#define BUF_SIZE 1024

#define DEBUG FALSE 
// 将十六进制字符串转换为UTF-8字符串
char* hexToUtf8(const char* hexString,int hexLength) {
    // printf("hexLength=> %d\n",hexLength);
    int utf8Length = hexLength / 2;
    char* utf8String = (char*) malloc(utf8Length + 1);

    int i, j;
    int colorCount = 0;
    for (i = 0, j = 0; i < hexLength; i += 2, j++) {
        char hexValue[3] = { hexString[i], hexString[i + 1], '\0' };
        int decimalValue = strtol(hexValue, NULL, 16);
        // printf("%c", (char) decimalValue);
        // if( (char)decimalValue== "§"){
        //     ++colorCount;
        // }else{
            utf8String[j] = (char) decimalValue;
        // }
    }

    utf8String[j] = '\0';
    return utf8String;
}
 
    void hexBufferToString(const unsigned char* buffer, size_t length, char* str_buffer) {
    for (size_t i = 0; i < length; i++) {
        sprintf(&str_buffer[i*2], "%02X", buffer[i]);
    }
} 

int main()
{
    WSADATA wsaData;
    SOCKET sock;
    SOCKADDR_IN serverAddr, clientAddr;
    char sendBuf[] = {0x01, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc1, 0x1d, 0x00, 0xff, 0xff, 0x00, 0xfe, 0xfe, 0xfe, 0xfe, 0xfd, 0xfd, 0xfd, 0xfd, 0x12, 0x34, 0x56, 0x78, 0x9c, 0x18, 0x28, 0x7f, 0xe1, 0x64, 0x89, 0x8d};
    char recvBuf[BUF_SIZE];
    int sendLen, recvLen, addrLen;

    // 初始化Winsock库
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed!\n");
        return 1;
    }

    // 创建UDP套接字
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    DWORD dwBytesReturned;
    BOOL bNewBehavior = FALSE;
    DWORD status;

    status = WSAIoctl(sock, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), NULL, 0, &dwBytesReturned, NULL, NULL);

    if (status == SOCKET_ERROR)
    {
        // 处理设置工作模式失败的情况
    }
    // 配置服务器地址信息
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 发送UDP数据包
    sendLen = sendto(sock, sendBuf, sizeof(sendBuf), 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (sendLen == SOCKET_ERROR) {
        printf("sendto failed with error: %ld\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    if(DEBUG)
    printf("UDP packet sent successfully!\n");

    // 设置超时时间为5秒
    struct timeval timeout;
    timeout.tv_sec = 5000;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    if(DEBUG)
    printf("UDP packet timeout set %d ms!\n",timeout.tv_sec);

    // 等待UDP回复
    addrLen = sizeof(clientAddr);
    recvLen = recvfrom(sock, recvBuf, BUF_SIZE, 0, (SOCKADDR*)&clientAddr, &addrLen);
    if (recvLen == SOCKET_ERROR) {
        printf("recvfrom failed with error: %ld\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }


    // 输出接收到的UDP回复
    if(DEBUG)
    printf("UDP packet received from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    recvLen -= 35;
    memmove(recvBuf,recvBuf+35,recvLen);
 

    char str_buffer[recvLen * 2 + 1]; // 预留一个额外的字符用于存储字符串结束符'\0'
    hexBufferToString(recvBuf, recvLen, str_buffer);
    // printf("\nend\n");
    // printf("\n");

    char* motd = hexToUtf8(str_buffer,recvLen * 2 + 1);
    // char* motd
    // printf("转换后的字符串: %s\n", motd);
    char *token;
    
    char *sign[] = {"服务器类型: ", "服务器名称: ", "协议=>", "版本=>", "在线人数: ", "最大人数: ","","地图名=> ", "默认模式: ", "默认模式: ", "IPV4端口: ",  "IPV6端口: ", "0=" };
    char pos = 0; 

    // 使用 strtok 函数分割字符串
    token = strtok(motd, ";");

    // 循环打印分割后的子字符串
    while (token != NULL) {
        printf("%s%s\n",sign[pos++], token);
        // printf("%s\n",  token);
        token = strtok(NULL, ";");
    }
    // printf("\n接收到的字符串");
    // printf("\n %c",newchar);



    // printf("\n");
    // printf("\n");
    // printf("\n");

    // 关闭UDP套接字
    closesocket(sock);
    // free(newchar);
    // 卸载Winsock库
    WSACleanup();

    return 0;
}
