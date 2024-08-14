#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <string>

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
	WSAData wsaData;

	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	char received[charMax];
	string sent;


	// Winsock 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		errorMessage("WSAStartup()");
	}

	//TCP 초기화
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		errorMessage("socket()");
	}
	currentMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));

	//서버 주소 구조체 설정
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(serverPort); // 2바이트 정수 네트워크 바이트 형식으로
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // 문자열 IP를 네트워크 바이트 형식으로

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		errorMessage("connect()");
	}
	currentMessage("connect()");

	// 반복적으로 서버로 메세지를 전송하고 에코 메세지를 전달 받는다.
	while (1) {
		cout << "[동엽]: ";
		getline(cin, sent);
		
		if (sent == "") {
			continue;
		}

		send(clientSocket, sent.c_str(), sent.length(), 0);

		int length = recv(clientSocket, received, sizeof(received)-1, 0);
		received[length] = '\0';

		if (strcmp(received, "exit") == 0) {
			cout << "[----------------서버 종료----------------]\n";
			break;
		}
		//cout << "[서버 메세지]: " << received << '\n';
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");

	return 0;
}