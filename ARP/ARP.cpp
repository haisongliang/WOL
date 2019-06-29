// ARP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#include <iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

int main()
{
	DWORD dwRetVal;
	IPAddr DestIp = 0;
	IPAddr SrcIp = 0;
	ULONG MacAddr[2];       /* for 6-byte hardware addresses */
	ULONG PhyAddrLen = 6;  /* default to length of six bytes */

	BYTE* bPhysAddr;

	char ip_buff[64] = "";
	for (int ip = 99; ip < 254; ip++)
	{
		memset(ip_buff, 0, sizeof(ip_buff));
		sprintf_s(ip_buff, "192.168.1.%d", ip + 1);
		DestIp = inet_addr(ip_buff);

		memset(&MacAddr, 0xff, sizeof(MacAddr));

		printf("Sending ARP request for IP address: %s\n", ip_buff);

		dwRetVal = SendARP(DestIp, SrcIp, &MacAddr, &PhyAddrLen);

		if (dwRetVal == NO_ERROR) {
			bPhysAddr = (BYTE*)& MacAddr;
			if (PhyAddrLen) {
				for (int i = 0; i < (int)PhyAddrLen; i++) 
				{
					if (i == (PhyAddrLen - 1))
						printf("%.2X\n", (int)bPhysAddr[i]);
					else
						printf("%.2X-", (int)bPhysAddr[i]);
				}
			}
			else
				printf
				("Warning: SendArp completed successfully, but returned length=0\n");

		}
		else {
			printf("Error: SendArp failed with error: %d", dwRetVal);
			switch (dwRetVal) {
			case ERROR_GEN_FAILURE:
				printf(" (ERROR_GEN_FAILURE)\n");
				break;
			case ERROR_INVALID_PARAMETER:
				printf(" (ERROR_INVALID_PARAMETER)\n");
				break;
			case ERROR_INVALID_USER_BUFFER:
				printf(" (ERROR_INVALID_USER_BUFFER)\n");
				break;
			case ERROR_BAD_NET_NAME:
				printf(" (ERROR_GEN_FAILURE)\n");
				break;
			case ERROR_BUFFER_OVERFLOW:
				printf(" (ERROR_BUFFER_OVERFLOW)\n");
				break;
			case ERROR_NOT_FOUND:
				printf(" (ERROR_NOT_FOUND)\n");
				break;
			default:
				printf("\n");
				break;
			}
		}
	}

	system("pause");

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
