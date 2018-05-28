#include "UDP.h"

bool getHostIp(char *ip)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		return false;
	}
	char hostname[256];
	if (SOCKET_ERROR == gethostname(hostname, sizeof(hostname)))
	{
		return false;
	}
	HOSTENT *host = gethostbyname(hostname);
	if (NULL == host)
	{
		return false;
	}
	strcpy(ip, inet_ntoa(*reinterpret_cast<in_addr*>(*host->h_addr_list)));
	return true;
}

bool serverOperation()
{
	SOCKET sClient;
	sockaddr_in clientAddr, bindAddr;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData))
	{
		return false;
	}
	sClient = socket(AF_INET, SOCK_DGRAM, 0);
	bool optval = true;
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bindAddr.sin_port = htons(SERVER_PORT);
	setsockopt(sClient, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char FAR*>(&optval), sizeof(optval));
	bind(sClient, reinterpret_cast<sockaddr*>(&bindAddr), sizeof(sockaddr_in));

	int addrLen = sizeof(SOCKADDR);
	char buf[256] = { 0 };
	char ipaddr[30] = { 0 };
	getHostIp(ipaddr);
	while (true)
	{
		if (SOCKET_ERROR != recvfrom(sClient, buf, 256, 0, reinterpret_cast<struct sockaddr FAR*>(&clientAddr), reinterpret_cast<int FAR*>(&addrLen)))
		{
			if (strcmp(buf, "GET_HOST_IP") == 0)
			{
				if (SOCKET_ERROR == sendto(sClient, ipaddr, 30, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), addrLen))
				{
					return false;
				}
				/*在此让最大连接数-1*/
			}
			else
			{
				continue;
			}
		}
		Sleep(1000);
	}
	return true;
}

bool clientOperation(char *hostIp)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		return false;
	}
	SOCKET connectSocket;
	connectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(CLIENT_PORT);
	sin.sin_addr.s_addr = 0;

	SOCKADDR_IN sinFrom;
	sinFrom.sin_family = AF_INET;
	sinFrom.sin_port = htons(SERVER_PORT);
	sinFrom.sin_addr.s_addr = INADDR_BROADCAST;

	bool optval = true;
	setsockopt(connectSocket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&optval), sizeof(optval));
	
	if (SOCKET_ERROR == bind(connectSocket, reinterpret_cast<SOCKADDR*>(&sin), sizeof(SOCKADDR)))
	{
		return false;
	}

	int addrLen = sizeof(SOCKADDR);
	char order[] = "GET_HOST_IP";
	//char hostIp[30] = { 0 };

	if (SOCKET_ERROR == sendto(connectSocket, order, strlen(order), 0, reinterpret_cast<sockaddr*>(&sinFrom), sizeof(sinFrom)))
	{
		return false;
	}
	while (true)
	{
		if (SOCKET_ERROR == recvfrom(connectSocket, hostIp, 30, 0, reinterpret_cast<SOCKADDR*>(&sinFrom), &addrLen))
		{
			return false;
		}
		if (strlen(hostIp))
		{
			break;
		}
	}
	return true;
}