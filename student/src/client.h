#pragma once


#include <WS2tcpip.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include "streamDataType.h"


class Client {
public:
	int socketFd;
	int connectionResult;
	Client() {
        // windows spercial
        #ifndef __APPLE__
                WSADATA wsaData;
                WSAStartup(MAKEWORD(2, 2), &wsaData);
        #endif

        this->socketFd = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        // 127.0.0.1
        serverAddress.sin_addr.s_addr = 0x0100007F;
        // 48815
        serverAddress.sin_port = 0xBEAF;

        this->connectionResult = connect(this->socketFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
		//If no error occurs, connect returns zero.
	}


    void login() {
        // todo 暂时可以直接连接, 还没做验证, 不需要账号密码
        int buffSize = 5;
        char* buffData = new char[buffSize];
        int dataSize = 0;
        buffData[0] = dataSize >> 0;
        buffData[1] = dataSize >> 8;
        buffData[2] = dataSize >> 16;
        buffData[3] = dataSize >> 24;
        buffData[4] = CONNECTIONREQUESTTYPE;
        send(this->socketFd, buffData, buffSize, 0);
        delete[] buffData;

    }


	static int readDataSize(int connectionId) {
		const int dataSizeInBytes = 4;
		char* header = new char[dataSizeInBytes];
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

	static char* readStreamData(int connectionId, int& dataSize, int& dataType) {
		int size = Client::readDataSize(connectionId);
		dataSize = size;
		int type = Client::readDataType(connectionId);
		dataType = type;
		if (size == 0) {
			return NULL;
		}
		char* data = new char[size];
		// 忘记收真正的数据了, fuck, 居然造成逻辑错误, debug 能力很重要啊
		recv(connectionId, data, size, MSG_WAITALL);
		return data;
	}

	bool loginResult() {
		int size;
		int type;
		char* buff = Client::readStreamData(this->socketFd, size, type);
		if (type == CONNECTIONRESPONSETYPE) {
			// connection response status, use the six byte
			// 0 represent success, 1 represent fail
			int result = buff[0];
			// 要先 delete 掉, 放后面的话, 由于 return ,就不会 delete 了
			// c++, 内存问题, 哎
			delete[] buff;
			if (result == 0) {
				return true;
			}
			else if (result == 1) {
				return false;
			}
		}
		else {
			// c++, 内存问题, 哎
			delete[] buff;
			return false;
		}
	}

private:

};

