#pragma comment(lib, "ws2_32.lib") // win sock
#include <winsock2.h> //using 2nd version of winsock
#include <iostream>
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)

SOCKET Connection;

enum Packet {
	P_ChatMessage, // type of packet
	P_Test

};

bool ProcessPacket(Packet packettype) {
	switch (packettype) {
	case P_ChatMessage:
	{
		int msg_size;
		recv(Connection, (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connection, msg, msg_size, NULL);
		std::cout << msg << std::endl;
		delete[] msg;
		break;
	}
	case P_Test:
		std::cout << "Test packet.\n";
		break;
		delete[] msg;
		break;

	default:
		std::cout << "Unrecognized packet: " << packettype << std::endl;
		break;
	}
	return true;
}

void ClientHandler() {
	Packet packettype;
	while (true) {
		recv(Connection, (char*)&packettype, sizeof(Packet), NULL);
		
		if (!ProcessPacket(packettype)) {
			break;
		}
	}
	closesocket(Connection);
	
}

int main(int arg, char* argv[]) {  //check if lib loaded 
	//WSASTARTUP
	WSADATA wsaData;
	WORD DLLVersion = MAKEWORD(2, 1); //lib version
	if (WSAStartup(DLLVersion, &wsaData) != 0) { // link to struc wsadata
		std::cout << "Error" << std::endl;
		exit(1);
	}


	SOCKADDR_IN addr; //sockaddr is for saving address // name addr
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //sin_addr is structure of sockaddr_in
	addr.sin_port = htons(1111); //port 
	addr.sin_family = AF_INET; //internet protocol AF_INET

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		std::cout << "Error: Failed connect to server.\n";
		return 1;
	}
	std::cout << "connected!\n";

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL); // multithreading

	std::string msg1;
	while (true) {
		std::getline(std::cin, msg1);
		Packet packettype = P_ChatMessage;
		send(Connection, (char*)&packettype, sizeof(Packet), NULL);
		int msg_size = msg1.size();
		send(Connection, (char*)&msg_size, sizeof(int), NULL);
		send(Connection, msg1.c_str(), msg_size, NULL); //c_str() string to char, 
		Sleep(10);
	}

	system("pause");
		return 0;
}