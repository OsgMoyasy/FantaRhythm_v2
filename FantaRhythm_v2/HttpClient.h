#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <WS2tcpip.h>
#include<fstream>



class HttpClient {
private:
	WSADATA wsaData;
	struct sockaddr_in server;
	SOCKET sock;
	char buf[65536];
	unsigned int** addrptr;

	std::string statusMassage;
	std::string result;

	std::string filepath;

	void jsonWriter();

public:
	HttpClient();
	~HttpClient();
	void testSend();
	void request(int chaNum[4]);

	void testPost(std::string postMassage);//テストPOST

	std::string getStatusMassage();
	std::string getResult();
	std::string getResultJsonString();//json形式に整形された形で返す
	std::string getFilePath();
};