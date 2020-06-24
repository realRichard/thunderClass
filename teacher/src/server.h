#pragma once


#include <WS2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <vector>


class Server {
public:
	int socketFd;
	//int connection;
	std::vector<int> connections;
	Server() {
		// windows spercial
		#ifndef __APPLE__
				WSADATA wsaData;
				WSAStartup(MAKEWORD(2, 2), &wsaData);
		#endif
		this->socketFd = socket(AF_INET, SOCK_STREAM, 0);
		// ip and port
		struct sockaddr_in serverAddress;
		serverAddress.sin_family = AF_INET;
		// 127.0.0.1
		serverAddress.sin_addr.s_addr = 0x0100007F;
		// 48815
		serverAddress.sin_port = 0xBEAF;

		bind(socketFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

		listen(socketFd, 6);
	}

	static void validateSuccessResponse(int connection) {
		int buffSize = 6;
		char* buffData = new char[buffSize];
		int size = 1;
		buffData[0] = size >> 0;
		buffData[1] = size >> 8;
		buffData[2] = size >> 16;
		buffData[3] = size >> 24;
		buffData[4] = CONNECTIONRESPONSETYPE;
		// connection response status
		// 0 represent success, 1 represent fail
		buffData[5] = 0;
		send(connection, buffData, buffSize, 0);
		delete[] buffData;
	}

	static void validateFailResponse(int connection) {
		int buffSize = 6;
		char* buffData = new char[buffSize];
		int size = 1;
		buffData[0] = size >> 0;
		buffData[1] = size >> 8;
		buffData[2] = size >> 16;
		buffData[3] = size >> 24;
		buffData[4] = CONNECTIONRESPONSETYPE;
		// connection response status, use the six byte
		// 0 represent success, 1 represent fail
		buffData[5] = 1;
		send(connection, buffData, buffSize, 0);
		delete[] buffData;
	}

	 //vector 值的传递和引用的传递是有区别的, 类似于 swap(), 所以导致后面 server->connections.size() 一直都是 0
	//static void connectionProcess(int socketFd, std::vector<int> connections) {
	//	while (true) {
	//		struct sockaddr_in clientAddress;
	//		socklen_t len = sizeof(clientAddress);
	//		// accept 会停在这里等待客户端连接上了再返回
	//		int connection = accept(socketFd, (struct sockaddr*)&clientAddress, &len);
	//		if (Server::validate(connection)) {
	//			connections.push_back(connection);
	//			Server::validateSuccessResponse(connection);
	//		}
	//		else {
	//			Server::validateFailResponse(connection);
	//		}
	//	}
	//}


	static void connectionProcess(int socketFd, std::vector<int> *connections) {
		while (true) {
			struct sockaddr_in clientAddress;
			socklen_t len = sizeof(clientAddress);
			// accept 会停在这里等待客户端连接上了再返回
			int connection = accept(socketFd, (struct sockaddr*)&clientAddress, &len);
			if (Server::validate(connection)) {
				//connections->push_back(connection);
				(*connections).push_back(connection);
				Server::validateSuccessResponse(connection);
			}
			else {
				Server::validateFailResponse(connection);
			}
		}
	}

	static int readDataSize(int connectionId) {
		const int dataSizeInBytes = 4;
		char *header = new char[dataSizeInBytes];
		recv(connectionId, header, dataSizeInBytes, MSG_WAITALL);
		int size = 0;
		size += header[0] << 0;
		size += header[1] << 8;
		size += header[2] << 16;
		size += header[3] << 24;
		delete[] header;
		return size;
	}

	static int readDataType(int connectionId) {
		const int dataTypeInBytes = 1;
		char* header = new char[dataTypeInBytes];
		recv(connectionId, header, dataTypeInBytes, MSG_WAITALL);
		int type = header[0];
		delete[] header;
		return type;
	}

	static char* readStreamData(int connectionId, int &dataSize, int &dataType) {
		int size = Server::readDataSize(connectionId);
		dataSize = size;
		int type = Server::readDataType(connectionId);
		dataType = type;
		if (size == 0) {
			return NULL;
		}
		char* data = new char[size];
		// 忘记收真正的数据了, fuck, 居然造成逻辑错误, debug 能力很重要啊
		recv(connectionId, data, size, MSG_WAITALL);
		return data;
	}

	static bool validate(int connection) {
		int size;
		int type;
		char* buff = Server::readStreamData(connection, size, type);
		if (type == CONNECTIONREQUESTTYPE) {

			// todo, 还没有做验证登入功能, 暂时所有都 push 进 connections

			delete[] buff;
			return true;
		}
		else {
			delete[] buff;
			return false;
		}
	}

	void sendPixels(int socketId, char *compressedPixels, int compressedPixelsSize) {
		send(socketId, compressedPixels, compressedPixelsSize, 0);
	}

	void sendAudio(int socketId, char *audioData, int audioLen) {
		send(socketId, audioData, audioLen, 0);
	}

private:

};