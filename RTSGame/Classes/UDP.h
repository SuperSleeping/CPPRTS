#include <WinSock2.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

#define SERVER_PORT 6000
#define CLIENT_PORT 6001

bool getHostIp(char *ip);
bool serverOperation();
bool clientOperation(char *hostIp);