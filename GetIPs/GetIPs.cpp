/*只获取ip v4*/
#include <iostream>
using namespace std;
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib")
void main()
{
	//初始化:如果不初始化，以下代码将无法执行
	WSAData data;
	if (WSAStartup(MAKEWORD(1, 1), &data) != 0)
	{
		cout << "初始化错误,无法获取主机信息..." << endl;
	}

	char host[255];
	//获取主机名:也可以使用GetComputerName()这个函数
	if (gethostname(host, sizeof(host)) == SOCKET_ERROR)
	{
		cout << "无法获取主机名..." << endl;
	}
	else
	{
		cout << "本机计算机名为:" << host << endl;
	}

	//获取计算机IP:gethostbyname也需要初始化(上面已初始化)
	struct hostent *p = gethostbyname(host);
	if (p == 0)
	{
		cout << "无法获取计算机主机名及IP..." << endl;
	}
	else
	{
		//获取本机计算机名
		//cout<<"本机计算机名为:"<<p->h_name<<endl;

		//本机IP:利用循环,输出本机所有IP
		for (int i = 0; p->h_addr_list[i] != 0; i++)
		{
			struct in_addr in;
			memcpy(&in, p->h_addr_list[i], sizeof(struct in_addr));
			cout << "第" << i + 1 << "块网卡的IP为:" << inet_ntoa(in) << endl;
		}

	}

	WSACleanup();

	cin.get();
}
