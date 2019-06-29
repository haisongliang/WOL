// WOL.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

#pragma comment(lib,"Ws2_32.lib")

int main()
{
	//初始化网络
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建socket
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		fprintf(stderr, "Socket create error: %d\n", GetLastError());
		return 0;
	}

	//设置为广播发送
	BOOL bOptVal = TRUE;
	int iOptLen = sizeof(BOOL);
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)& bOptVal, iOptLen) == SOCKET_ERROR)
	{
		fprintf(stderr, "setsockopt error: %d\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return 0;
	}

	sockaddr_in to;
	to.sin_family = AF_INET;
	to.sin_port = htons(0);
	to.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	vector<string> macs;
	ifstream ifs("macs.txt");
	string macStr;
	if (ifs)
	{
		while (!ifs.eof())
		{
			ifs >> macStr >> macStr;
			macs.push_back(macStr);
		}
	}

	for (int jj = 0; jj < macs.size(); jj++)
	{
		const char* mac = macs[jj].c_str();
		int dstaddr[6];
		int i = sscanf_s(mac, "%2x-%2x-%2x-%2x-%2x-%2x",
			&dstaddr[0], &dstaddr[1], &dstaddr[2], &dstaddr[3], &dstaddr[4], &dstaddr[5]);
		if (i != 6)
		{
			fprintf(stderr, "Invalid MAC Adresse!");
			return 0;
		}

		unsigned char ether_addr[6];
		for (i = 0; i < 6; i++)
			ether_addr[i] = dstaddr[i];

		//构造Magic Packet
		u_char magicpacket[200];
		memset(magicpacket, 0xff, 6);
		int packetsize = 6;
		for (i = 0; i < 16; i++)
		{
			memcpy(magicpacket + packetsize, ether_addr, 6);
			packetsize += 6;
		}
		//发送Magic Packet
		if (sendto(sock, (const char*)magicpacket, packetsize, 0, (const struct sockaddr*) & to, sizeof(to)) == SOCKET_ERROR)
			fprintf(stderr, "Magic packet send error: %d\n", WSAGetLastError());
		else
			printf("Magic packet send!\n");
	}

	closesocket(sock);

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
