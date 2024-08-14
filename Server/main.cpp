#include <iostream>
#include <cstring>
#include <WinSock2.h>

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
	WSADATA wsaData;

	SOCKET serverSocket;
	SOCKET clientSocket;

	SOCKADDR_IN serverAddress;
	SOCKADDR_IN clientAddress;

	char received[charMax];

	//Winsock �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		errorMessage("WSAStartup");
	}

	//TCP ���� ����
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == INVALID_SOCKET) {
		errorMessage("socket()");
	}
	currentMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));

	//���� �ּ� ����ü ����
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(serverPort); // 2����Ʈ ���� ��Ʈ��ũ ����Ʈ ��������
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //4����Ʈ ���� ��Ʈ��ũ ����Ʈ ��������

	//bind �Լ�, ���Ͽ� ���� �ּҿ� ��Ʈ�� �Ҵ�
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
		errorMessage("bind()");
	}
	currentMessage("bind()");


	//listen �Լ�, Ŭ���̾�Ʈ�κ��� ���� ��û�� ��ٸ���.
	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		errorMessage("listen()");
	}
	currentMessage("listen()");

	/* ���� ����
	* 
	//accept �Լ�, ���� ��� ���� ���� ��û�� �����Ѵ�.
	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	currentMessage("accept()");

	if (clientSocket == INVALID_SOCKET) {
		errorMessage("accept()");
	}

	// Ŭ���̾�Ʈ �޼��� �ޱ�
	while (1) {
		int length = recv(clientSocket, received, sizeof(received)-1, 0);
		received[length] = NULL;

		cout << "[Client Messasge]: " << received << '\n';
		cout << "[Send Message]: " << received << '\n';

		if (strcmp(received, "exit") == 0) {
			send(clientSocket, received, sizeof(received), 0);
			cout << "[----------------���� ����----------------]\n";
			break;
		}
		send(clientSocket, received, sizeof(received), 0);
	}
	*/

	while (1) {
		int sizeClientAddress = sizeof(clientAddress);

		// accept �Լ�, ���� ��� ���� ���� ��û ����
		clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
		if (clientSocket == INVALID_SOCKET) {
			errorMessage("accept()");
		}
		currentMessage("accept()");

		while (1) {
			int length = recv(clientSocket, received, sizeof(received) - 1, 0);
			if (length <= 0) {
				currentMessage("Ŭ���̾�Ʈ ���� ����");
				break;
			}
			received[length] = '\0';

			cout << "[Client Message]: " << received << '\n';

			if (strcmp(received, "exit") == 0) {
				send(clientSocket, received, sizeof(received), 0);
				cout << "[----------------���� ����----------------]\n";
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
