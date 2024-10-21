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

	// 파일 열기
	FILE* fp;
	fopen_s(&fp, "simple.txt", "rb");

	if (fp == NULL)
	{
		perror("파일 입출력 오류");
		return -1;
	}

	// 파일 이름 보내기
	char filename[FileNameSize];
	ZeroMemory(filename, FileNameSize);
	sprintf_s(filename, "simple.txt");
	retval = send(sock, filename, FileNameSize, 0);
	if (retval == SOCKET_ERROR) error::Quit(L"send()");

	// 파일 크기 얻기
	fseek(fp, 0, SEEK_END);
	int totalbytes = ftell(fp);

	// 파일 크기 보내기
	retval = send(sock, (char*)&totalbytes, sizeof(totalbytes), 0);
	if (retval == SOCKET_ERROR) error::Quit(L"send()");

	// 파일 데이터 전송에 사용할 변수
	char buffer[BUFSIZE];
	int numRead;
	int numTotal = 0;

	// 파일 데이터 보내기
	rewind(fp); // 파일 포이터를 제일 앞으로 이동
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
			printf("파일 전송완료!: %d 바이트\n", numTotal);
			break;
		}
		else
		{
			perror("파일 입출력 오류");
			break;
		}
	}
	fclose(fp);


	// closesocket()
	closesocket(sock);

	//윈속 종료
	WSACleanup();
	return 0;
}