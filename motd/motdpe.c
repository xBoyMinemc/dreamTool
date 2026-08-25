// gcc motdpe.c -o motdpe.exe -lws2_32
// chcp 65001
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")

#ifndef SIO_UDP_CONNRESET
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)
#endif

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 19132
#define BUF_SIZE 2048

#define DEBUG FALSE

int main(int argc, char *argv[])
{
    // 设置 Windows 控制台为 UTF-8 编码，防止中文乱码
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    const char *server_ip = (argc > 1) ? argv[1] : DEFAULT_IP;
    int server_port = (argc > 2) ? atoi(argv[2]) : DEFAULT_PORT;

    WSADATA wsaData;
    SOCKET sock;
    SOCKADDR_IN serverAddr, clientAddr;

    // RakNet Unconnected Ping (ID: 0x01, Timestamp: 8B, Magic: 16B, ClientGUID: 8B)
    char sendBuf[] = {
        0x01,
        0x00, 0x00, 0x00, 0x00, 0x21, 0x90, 0xee, 0x82,
        0x00, 0xff, 0xff, 0x00, 0xfe, 0xfe, 0xfe, 0xfe, 0xfd, 0xfd, 0xfd, 0xfd, 0x12, 0x34, 0x56, 0x78,
        0x9c, 0x18, 0x28, 0x7f, 0xe1, 0x64, 0x89, 0x8d
    };
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

    // 禁用 WSAECONNRESET (Windows UDP 10054 错误忽略)
    DWORD dwBytesReturned = 0;
    BOOL bNewBehavior = FALSE;
    WSAIoctl(sock, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), NULL, 0, &dwBytesReturned, NULL, NULL);

    // 设置接收超时时间为 3000 ms (Windows Winsock 使用 DWORD 毫秒)
    DWORD timeout = 3000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    // 配置服务器地址信息
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons((unsigned short)server_port);
    serverAddr.sin_addr.s_addr = inet_addr(server_ip);

    printf("Connecting to %s:%d ...\n", server_ip, server_port);

    // 发送UDP数据包
    sendLen = sendto(sock, sendBuf, sizeof(sendBuf), 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (sendLen == SOCKET_ERROR) {
        printf("sendto failed with error: %ld\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 等待UDP回复
    addrLen = sizeof(clientAddr);
    recvLen = recvfrom(sock, recvBuf, BUF_SIZE - 1, 0, (SOCKADDR*)&clientAddr, &addrLen);
    if (recvLen == SOCKET_ERROR) {
        printf("recvfrom failed with error: %ld (Timeout or Server Offline)\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // RakNet Unconnected Pong (ID 0x1c):
    // 1B ID + 8B PingTime + 8B ServerGUID + 16B Magic + 2B StringLength = 35 Bytes
    if (recvLen <= 35 || (unsigned char)recvBuf[0] != 0x1c) {
        printf("Invalid packet received or packet too short (length: %d)\n", recvLen);
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    int motdLen = recvLen - 35;
    char *motd = recvBuf + 35;
    motd[motdLen] = '\0'; // 确保字符串以 \0 结尾

    printf("\n=== MOTD 原始数据 ===\n%s\n\n=== 解析结果 ===\n", motd);

    const char *sign[] = {
        "服务器类型: ",
        "服务器名称: ",
        "协议版本: ",
        "游戏版本: ",
        "在线人数: ",
        "最大人数: ",
        "Server GUID: ",
        "副标题/地图: ",
        "游戏模式: ",
        "模式数值: ",
        "IPv4 端口: ",
        "IPv6 端口: ",
        "额外参数1: ",
        "额外参数2: ",
        "额外参数3: ",
        "额外参数4: "
    };
    int maxSigns = sizeof(sign) / sizeof(sign[0]);
    int pos = 0;

    // 使用 strtok 分割字符串
    char *token = strtok(motd, ";");
    while (token != NULL) {
        if (pos < maxSigns) {
            printf("%-14s %s\n", sign[pos], token);
        } else {
            printf("扩展字段[%02d]:  %s\n", pos, token);
        }
        pos++;
        token = strtok(NULL, ";");
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}