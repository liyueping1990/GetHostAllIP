/*ֻ��ȡip v4*/
#include <iostream>
using namespace std;
#include "winsock2.h"
#pragma comment(lib,"ws2_32.lib")
void main()
{
	//��ʼ��:�������ʼ�������´��뽫�޷�ִ��
	WSAData data;
	if (WSAStartup(MAKEWORD(1, 1), &data) != 0)
	{
		cout << "��ʼ������,�޷���ȡ������Ϣ..." << endl;
	}

	char host[255];
	//��ȡ������:Ҳ����ʹ��GetComputerName()�������
	if (gethostname(host, sizeof(host)) == SOCKET_ERROR)
	{
		cout << "�޷���ȡ������..." << endl;
	}
	else
	{
		cout << "�����������Ϊ:" << host << endl;
	}

	//��ȡ�����IP:gethostbynameҲ��Ҫ��ʼ��(�����ѳ�ʼ��)
	struct hostent *p = gethostbyname(host);
	if (p == 0)
	{
		cout << "�޷���ȡ�������������IP..." << endl;
	}
	else
	{
		//��ȡ�����������
		//cout<<"�����������Ϊ:"<<p->h_name<<endl;

		//����IP:����ѭ��,�����������IP
		for (int i = 0; p->h_addr_list[i] != 0; i++)
		{
			struct in_addr in;
			memcpy(&in, p->h_addr_list[i], sizeof(struct in_addr));
			cout << "��" << i + 1 << "��������IPΪ:" << inet_ntoa(in) << endl;
		}

	}

	WSACleanup();

	cin.get();
}
