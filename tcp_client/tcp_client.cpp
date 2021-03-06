// tcp_client.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

HANDLE hThread[2];
DWORD dwThreadID[2];
int sock, strcnt;
WSADATA wsa;
unsigned int WINAPI ThreadProc(LPVOID lpParam);
unsigned int WINAPI RecvProc(LPVOID lpParam);

#define BUFFERS 1024
int main(int argc, char**argv) {
	
	DWORD cnt_thread = 0;

	struct sockaddr_in serv_addr;
	if (argc != 3) {
		printf("Usage : %s <ip> <port>", argv[0]);
		exit(1);
	}


	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		return -1;
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		printf("socket() error...");
		exit(1);
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
		printf("Connect() error...");
		exit(1);
	}

	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, (LPVOID)cnt_thread, 0, (unsigned int*)&dwThreadID[0]);
	ResumeThread(hThread[0]);

	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, RecvProc, (LPVOID)cnt_thread, 0, (unsigned int*)&dwThreadID[1]);
	ResumeThread(hThread[1]);

	WaitForSingleObject(hThread[0], INFINITE);
	//close(sock);
	closesocket(sock);
	WSACleanup();

	return 0;
}

static int async = 0;
unsigned int WINAPI ThreadProc(LPVOID lpParam)
{
	char msg[BUFFERS];

	while (1) {
		if (async == 0) {
			fputs("Send Message (q to exit) : ", stdout);
			fgets(msg, BUFFERS, stdin);
			if (!strcmp(msg, "q\n")) break;

			send(sock, msg, strlen(msg), 0);
			async = 1;
		}
	}
	return 1;
}

unsigned int WINAPI RecvProc(LPVOID lpParam)
{
	char msg[BUFFERS];

	while (1) {
		strcnt = recv(sock, msg, BUFFERS - 1, 0);

		msg[strcnt] = 0;

		printf("\nRecv Message from Server : %s", msg);
		memset(msg, 0, sizeof(msg));
		async = 0;
	}
	return 1;
}

