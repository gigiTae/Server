#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h> 
#include <stdlib.h>
#include <stdio.h>
#include "Error.h"
#include "uitll.h"
#pragma comment(lib, "Ws2_32.lib")


constexpr size_t BUFSIZE = 512;
constexpr size_t FileNameSize = 256;

int main(int argc, char* argv[])
{
	int retval;
	
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1; 

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) error::Quit(L"socket()");

	// connect()
	SOCKADDR_IN serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(9000);
	serverAddress.sin_addr.s_addr = inet_addr("59.13.235.198");
	retval = connect(sock, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	if (retval == SOCKET_ERROR) error::Quit(L"connect()");

	// ���� ����
	FILE* fp;
	fopen_s(&fp, "simple.txt", "rb");

	if (fp == NULL)
	{
		perror("���� ����� ����");
		return -1;
	}

	// ���� �̸� ������
	char filename[FileNameSize];
	ZeroMemory(filename, FileNameSize);
	sprintf_s(filename, "simple.txt");
	retval = send(sock, filename, FileNameSize, 0);
	if (retval == SOCKET_ERROR) error::Quit(L"send()");

	// ���� ũ�� ���
	fseek(fp, 0, SEEK_END);
	int totalbytes = ftell(fp);

	// ���� ũ�� ������
	retval = send(sock, (char*)&totalbytes, sizeof(totalbytes), 0);
	if (retval == SOCKET_ERROR) error::Quit(L"send()");

	// ���� ������ ���ۿ� ����� ����
	char buffer[BUFSIZE];
	int numRead;
	int numTotal = 0;

	// ���� ������ ������
	rewind(fp); // ���� �����͸� ���� ������ �̵�
	while (true)
	{
		numRead = fread(buffer, 1, BUFSIZE, fp);
		if (numRead > 0)
		{
			retval = send(sock, buffer, numRead, 0);
			if (retval == SOCKET_ERROR)
			{
				error::Display(L"send()");
				break;
			}
			numTotal += numRead;
		}
		else if (numRead == 0 && numTotal == totalbytes)
		{
			printf("���� ���ۿϷ�!: %d ����Ʈ\n", numTotal);
			break;
		}
		else
		{
			perror("���� ����� ����");
			break;
		}
	}
	fclose(fp);


	// closesocket()
	closesocket(sock);

	//���� ����
	WSACleanup();
	return 0;
}