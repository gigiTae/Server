#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h> 
#include <stdlib.h>
#include <stdio.h>
#include "Error.h"
#include "uitll.h"
#pragma comment(lib, "Ws2_32.lib")


constexpr size_t BUFSIZE = 4096;
constexpr size_t FileNameSize = 256;

int main(int argc, char* argv[])
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) error::Quit(L"socket()");

	// bind
	SOCKADDR_IN serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(9000);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
	if (retval == SOCKET_ERROR) error::Quit(L"bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) error::Quit(L"listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clinentAddress;
	int addressLength;
	char buf[BUFSIZE];

	while (true)
	{
		// accept()
		addressLength = sizeof(clinentAddress);
		client_sock = accept(listen_sock, (SOCKADDR*)&clinentAddress, &addressLength);
		if (client_sock == INVALID_SOCKET)
		{
			error::Display(L"accept()");
			continue;
		}

		printf("\nFileSender 접속 : IP 주소=%s, 포트번호 %d\n",
			inet_ntoa(clinentAddress.sin_addr),
			ntohs(clinentAddress.sin_port));

		// 파일 이름 받기
		char filename[FileNameSize];
		ZeroMemory(filename, FileNameSize);
		retval = util::Recvn(client_sock, filename, FileNameSize, 0);
		if (retval == SOCKET_ERROR)
		{
			error::Display(L"recv()");
			closesocket(client_sock);
			continue;
		}

		printf("-> 받을 파일 이름 : %s\n", filename);

		// 파일 크기 받기
		int totalbytes;
		retval = util::Recvn(client_sock, (char*)&totalbytes, sizeof(totalbytes), 0);
		if (retval == SOCKET_ERROR)
		{
			error::Display(L"recv()");
			closesocket(client_sock);
			continue;
		}

		printf("-> 받을 파일 크기: %d\n", totalbytes);

		// 파일 열기
		FILE* fp;
		fopen_s(&fp, filename, "wb");
		
		if (fp == NULL)
		{
			perror("파일 입출력 오류");
			closesocket(client_sock);
			continue;
		}

		// 파일 데이터 받기
		int numTotal = 0;
		while (true)
		{
			retval = util::Recvn(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR)
			{
				error::Display(L"recvn()");
				break;
			}
			else if (retval == 0)
				break;
			else
			{
				fwrite(buf, 1, retval, fp);
				if (ferror(fp))
				{
					perror("파일 입출력 오류");
					break;
				}
				numTotal += retval;
			}
		}

		fclose(fp);

		// 전송 결과 출력
		if (numTotal == totalbytes)
			printf("-> 파일 전송 완료!\n");
		else
			printf("-> 파일 전송 실패!\n");

		closesocket(client_sock);
		printf("FileSender 종료 : IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clinentAddress.sin_addr),
			ntohs(clinentAddress.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	//윈속 종료
	WSACleanup();

	return 0;
}