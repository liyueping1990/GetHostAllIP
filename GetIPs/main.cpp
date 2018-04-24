/*同时获取ip v4和v6*/
#include <iostream>
#include <string>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define WIN32

#pragma comment(lib, "WS2_32.lib")

int32_t GetHostName(char* hostName, int32_t size)
{	
	int32_t result = -1;

#ifdef WIN32
	result = gethostname(hostName, size);
#endif
	return result;
}

int GetHostAddr(std::vector<std::string>& hostAddr)
{	
	char hostName[128] = { 0 };
#ifdef WIN32
	WSAData data;//初始化:如果不初始化，以下代码将无法执行
	if (WSAStartup(MAKEWORD(2, 1), &data) != 0)
	{
		std::cout << "初始化错误,无法获取主机信息..." << std::endl;
	}

	int32_t ret = GetHostName(hostName, 128);

	addrinfo hints;
	memset(&hints, 0, sizeof(addrinfo));
	addrinfo* addr = NULL;
	if (ret == 0)
	{
		getaddrinfo(hostName, NULL, &hints, &addr);
	}
	addrinfo* addrInfo = addr;

	while (addrInfo != NULL)
	{
		std::string sockAddr;
		if (addrInfo->ai_family == AF_INET || addrInfo->ai_family == PF_INET)
		{
			sockaddr_in addrin = *((sockaddr_in*)addrInfo->ai_addr);
			char*  ip = inet_ntoa(addrin.sin_addr);
			//int32_t port = htons(addrin.sin_port);

			sockAddr = ip;
		}
		else
		{
			sockaddr_in6 addrin6 = *((sockaddr_in6*)addrInfo->ai_addr);
			char ip[64] = { 0 };
			getnameinfo((sockaddr*)&addrin6, sizeof(addrin6), ip, 64, NULL, 0, NI_NUMERICHOST);
			//int32_t port = htons(addrin6.sin6_port);

			sockAddr = ip;
		}
		hostAddr.push_back(sockAddr);
		addrInfo = addrInfo->ai_next;
	}
	freeaddrinfo(addr);
	WSACleanup();
#else
	ifaddrs *ifaddr, *ifa;
	if (getifaddrs(&ifaddr) == -1)
	{
		printf("fatal error, can not get machine ip\n");
	}
	else
	{
		for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
		{
			if (ifa->ifa_addr == NULL)
				continue;
			int32_t family = ifa->ifa_addr->sa_family;
			int32_t size = (family == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
			int32_t ret = getnameinfo(ifa->ifa_addr, size, hostName, 128, NULL, 0, NI_NUMERICHOST);
			if (ret == 0)
			{
				std::string addr(hostName, 0);
				if (family != AF_INET)
				{
					addr.mMode = IPV6;
				}
				hostAddr.push_back(addr);
			}
		}
		freeifaddrs(ifaddr);
	}
#endif

	return 0;
}

int main()
{
	std::vector<std::string> ips;
	GetHostAddr(ips);
	return 0;
}