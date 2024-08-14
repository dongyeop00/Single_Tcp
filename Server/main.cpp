#include <iostream>
#include <cstring>
#include <WinSock2.h>

using namespace std;

#define serverPort 8080
#define charMax 256

#pragma comment(lib, "ws2_32.lib")

void errorMessage(string message) {
	cout << "[오류발생]: " << message << "\n";
	system("pause");
	exit(1);
}

void currentMessage(string message) {
	cout << "[현재상태]: " << message << "\n";
}


int main() {
	WSADATA wsaData;

	SOCKET serverSocket;
	SOCKET clientSocket;

	SOCKADDR_IN serverAddress;
	SOCKADDR_IN clientAddress;

	char received[charMax];

	//Winsock 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		errorMessage("WSAStartup");
	}

	//TCP 소켓 생성
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == INVALID_SOCKET) {
		errorMessage("socket()");
	}
	currentMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));

	//서버 주소 구조체 설정
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(serverPort); // 2바이트 정수 네트워크 바이트 형식으로
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //4바이트 정수 네트워크 바이트 형식으로

	//bind 함수, 소켓에 로컬 주소와 포트를 할당
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		errorMessage("bind()");
	}
	currentMessage("bind()");


	//listen 함수, 클라이언트로부터 연결 요청을 기다린다.
	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		errorMessage("listen()");
	}
	currentMessage("listen()");

	/* 단일 연결
	* 
	//accept 함수, 수신 대기 중인 연결 요청을 수락한다.
	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	currentMessage("accept()");

	if (clientSocket == INVALID_SOCKET) {
		errorMessage("accept()");
	}

	// 클라이언트 메세지 받기
	while (1) {
		int length = recv(clientSocket, received, sizeof(received)-1, 0);
		received[length] = NULL;

		cout << "[Client Messasge]: " << received << '\n';
		cout << "[Send Message]: " << received << '\n';

		if (strcmp(received, "exit") == 0) {
			send(clientSocket, received, sizeof(received), 0);
			cout << "[----------------서버 종료----------------]\n";
			break;
		}
		send(clientSocket, received, sizeof(received), 0);
	}
	*/

	while (1) {
		int sizeClientAddress = sizeof(clientAddress);

		// accept 함수, 수신 대기 중인 연결 요청 수락
		clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
		if (clientSocket == INVALID_SOCKET) {
			errorMessage("accept()");
		}
		currentMessage("accept()");

		while (1) {
			int length = recv(clientSocket, received, sizeof(received) - 1, 0);
			if (length <= 0) {
				currentMessage("클라이언트 연결 종료");
				break;
			}
			received[length] = '\0';

			cout << "[Client Message]: " << received << '\n';

			if (strcmp(received, "exit") == 0) {
				send(clientSocket, received, sizeof(received), 0);
				cout << "[----------------서버 종료----------------]\n";
				break;
			}
			send(clientSocket, received, sizeof(received), 0);
		}

		closesocket(clientSocket);
	}

	//closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");

	return 0;
}
