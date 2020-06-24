#pragma once


#ifdef __APPLE__
#include <sys/socket.h>
#include <arpa/inet.h>
#else
//#include <tchar.h>
#include <WS2tcpip.h>
#include <winsock2.h>
//#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")
#endif