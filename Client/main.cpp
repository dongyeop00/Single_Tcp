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
	cout << "[�����߻�]: " << message << "\n";
	system("pause");
	exit(1);
}

void currentMessage(string message) {
	cout << "[�������]: " << message << "\n";
}

int main() {
	WSAData wsaData;

	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	char received[charMax];
	string sent;


	// Winsock �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		errorMessage("WSAStartup()");
	}

	//TCP �ʱ�ȭ
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		errorMessage("socket()");
	}
	currentMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));

	//���� �ּ� ����ü ����
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(serverPort); // 2����Ʈ ���� ��Ʈ��ũ ����Ʈ ��������
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // ���ڿ� IP�� ��Ʈ��ũ ����Ʈ ��������

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		errorMessage("connect()");
	}
	currentMessage("connect()");

	// �ݺ������� ������ �޼����� �����ϰ� ���� �޼����� ���� �޴´�.
	while (1) {
		cout << "[����]: ";
		getline(cin, sent);
		
		if (sent == "") {
			continue;
		}

		send(clientSocket, sent.c_str(), sent.length(), 0);

		int length = recv(clientSocket, received, sizeof(received)-1, 0);
		received[length] = '\0';

		if (strcmp(received, "exit") == 0) {
			cout << "[----------------���� ����----------------]\n";
			break;
		}
		//cout << "[���� �޼���]: " << received << '\n';
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");

	return 0;
}