/*
���newgame��whileѭ����ʱ����
*/

#include "UDP.h"

bool getHostIp(char *ip)
{
	//�����׽���
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		return false;
	}
	//��ȡ������
	char hostname[256];
	if (SOCKET_ERROR == gethostname(hostname, sizeof(hostname)))
	{
		return false;
	}
	//��ȡ����ip
	HOSTENT *host = gethostbyname(hostname);
	if (NULL == host)
	{
		return false;
	}
	//����ip��ַ
	strcpy(ip, inet_ntoa(*reinterpret_cast<in_addr*>(*host->h_addr_list)));
	return true;
}

bool serverOperation(int players)
{
	int number = players - 1;
	SOCKET sClient, clients;
	sockaddr_in clientAddr, bindAddr, broadcastAddr;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData))
	{
		return false;
	}
	//UDP�׽���
	sClient = socket(AF_INET, SOCK_DGRAM, 0);
	clients = socket(AF_INET, SOCK_DGRAM, 0);
	//����Ϊ�㲥����
	bool optval = true;
	int opt = -1;
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bindAddr.sin_port = htons(SERVER_PORT);
	setsockopt(sClient, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char FAR*>(&optval), sizeof(optval));
	bind(sClient, reinterpret_cast<sockaddr*>(&bindAddr), sizeof(sockaddr_in));

	broadcastAddr.sin_family = AF_INET;
	broadcastAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	broadcastAddr.sin_port = htons(SERVER_PORT);
	setsockopt(clients, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char FAR*>(&opt), sizeof(opt));

	int addrLen = sizeof(SOCKADDR);
	char buf[256] = { 0 };			//������յ�������
	char ipaddr[30] = { 0 };		//���汾��ip
	getHostIp(ipaddr);
	//int num = 0;
	while (number)
	{
		if (SOCKET_ERROR != recvfrom(sClient, buf, 256, 0, reinterpret_cast<struct sockaddr FAR*>(&clientAddr), reinterpret_cast<int FAR*>(&addrLen)))
		{
			if (strcmp(buf, "GET_HOST_IP") == 0)
			{
				sendto(sClient, ipaddr, 30, 0, reinterpret_cast<SOCKADDR*>(&clientAddr), addrLen);
			}
			else if (strcmp(buf, "success") == 0)
			{
				//�ɹ���������
				number--;
				//break;
				//���ͻ������ӡ���ͻ���Ԥ�������������ڴ�--��
			}
			else
			{
				//��Чָ��
				continue;
			}
		}
		Sleep(100);
	}
	return true;
}

bool clientOperation(char *hostIp)
{
	bool flag = 0;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		return false;
	}
	SOCKET connectSocket;
	connectSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//���׽���
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(CLIENT_PORT);
	sin.sin_addr.s_addr = 0;

	//���ܹ㲥�ĵ�ַ
	SOCKADDR_IN sinFrom;
	sinFrom.sin_family = AF_INET;
	sinFrom.sin_port = htons(SERVER_PORT);
	sinFrom.sin_addr.s_addr = INADDR_BROADCAST;

	bool optval = true;
	setsockopt(connectSocket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&optval), sizeof(optval));
	
	bind(connectSocket, reinterpret_cast<SOCKADDR*>(&sin), sizeof(SOCKADDR));

	int addrLen = sizeof(SOCKADDR);
	char order[] = "GET_HOST_IP";
	char command[30];

	sendto(connectSocket, order, strlen(order), 0, reinterpret_cast<sockaddr*>(&sinFrom), sizeof(sinFrom));
	while (true)
	{
		if (SOCKET_ERROR != recvfrom(connectSocket, command, 30, 0, reinterpret_cast<SOCKADDR*>(&sinFrom), &addrLen))
		{
			/*if (strcmp(command, "begin") == 0)
			{
				break;
			}
			else*/ if (strlen(command))
			{
				strcpy(hostIp, command);
				sendto(connectSocket, "success", 8, 0, reinterpret_cast<sockaddr*>(&sinFrom), sizeof(sinFrom));
				break;
			}
		}
	}
	return true;
}