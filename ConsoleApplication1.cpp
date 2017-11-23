#include "stdafx.h"
#include <winsock2.h>
#pragma comment (lib,"WSock32.Lib")
#include <stdio.h>

int main()
{
	SOCKET socketS;

	int iResult;

	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		WSACleanup();
		return 0;
	}

	struct sockaddr_in local;
	struct sockaddr_in from;
	int fromlen = sizeof(from);
	local.sin_family = AF_INET;
	local.sin_port = htons(4242);
	local.sin_addr.s_addr = INADDR_ANY;

	socketS = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (socketS == INVALID_SOCKET) {
		printf("Error at socket");
		WSACleanup();
		return 0;
	}
	
	iResult = bind(socketS, (sockaddr*)&local, sizeof(local));

	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error");
		WSACleanup();
		return 0;
	}

	int bytes_read = 0;

	struct TOpenTrack {
			double x;
			double y;
			double z;
			double yaw;
			double pitch;
			double roll;
		};

		
	TOpenTrack OpenTrack;

	memset(&OpenTrack, 0, sizeof(OpenTrack));

	while (true)
	{

		bytes_read = recvfrom(socketS, (char*)(&OpenTrack), sizeof(OpenTrack), 0, (sockaddr*)&from, &fromlen);
	
		if (bytes_read > 0) 
			printf("%f %f %f\n", OpenTrack.yaw, OpenTrack.pitch, OpenTrack.roll);
			
		if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0) break;
		
	}

	closesocket(socketS);

	return 0;
}

