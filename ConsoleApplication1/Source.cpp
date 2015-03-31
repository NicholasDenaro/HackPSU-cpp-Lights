#include <iostream>
#include <WinSock2.h>
#include "Serial.h"
#include <string>

// link with Ws2_32.lib
#pragma comment(lib,"Ws2_32.lib")

using namespace std;

int main(int argc, char* args[])
{
	cout<<"Number of lights: "<<args[1]<<endl;

	int lightLength=stoi(args[1]);

    Serial serial = Serial("COM5");

	SOCKET sock = INVALID_SOCKET;
	int iFamily = AF_INET;
	int iType = SOCK_STREAM;
	int iProtocol = IPPROTO_TCP;
	WSADATA wsaData = { 0 };
	struct sockaddr_in clientService;

	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientService.sin_port = htons(7777);

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        wprintf(L"WSAStartup failed: %d\n", iResult);
        return 1;
    }

	sock = socket(iFamily, iType, iProtocol);
	//iResult = connect(sock, (SOCKADDR*)&clientService, sizeof(clientService));
	iResult = bind(sock, (SOCKADDR*)&clientService, sizeof(clientService));
	cout << "result...? " << iResult << endl;

	if (sock == INVALID_SOCKET)
		wprintf(L"socket function failed with error = %d\n", WSAGetLastError());
	else
	{
		if (listen(sock, 1) == SOCKET_ERROR) {
			wprintf(L"listen failed with error: %ld\n", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			system("pause");
			return 1;
		}
		wprintf(L"socket function succeeded\n");
		SOCKET connection;// = socket(iFamily, iType, iProtocol);
		wprintf(L"Waiting for client to connect...\n");
		connection=accept(sock, NULL, NULL);

		if (connection == INVALID_SOCKET) {
			wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
			closesocket(sock);
			WSACleanup();
			system("pause");
			return 1;
		}
		else
			wprintf(L"Client connected.\n");

		char* buf = new char[4]{'a','b','c','\0'};
		int ret;// = recv(connection, buf, 3, 0);
		int counter = 0;
        char i = 0;
		while (true)
		{
			while ((ret = recv(connection, buf, 3, 0)) == -1)
			{
				//cout << "\t--- " << buf << endl;
			};
			//cout << "read: " << ret << endl;
			//cout << "\t" << buf << endl;
            serial.WriteData(&i, 1);
            serial.WriteData(buf, 3);
			i++;
			//if (i >= 96) {
			if(i>=lightLength){
				i = 0;
			}
			//cout << "DataR: " << (int)buf[0] << " DataG: " << (int)buf[1] << " DataB: " << (int)buf[2] << endl;
			//cout << "Number of lights updated: " << counter++ << endl;
		}
	}
	
	system("pause");
}