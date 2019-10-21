#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <WS2tcpip.h>
#include<fstream>
#include <sstream>



class HttpClient {
private:
	WSADATA wsaData;
	struct sockaddr_in server;
	SOCKET sock;
	char buf[1024];
	unsigned int** addrptr;

	std::string statusMassage;
	std::string result;

	std::string filepath;

	void jsonWriter();

public:
	HttpClient();
	~HttpClient();
	void testGet();
	void characterDataRequest(int chaNum[4]);

	void testPost(std::string postMassage);//�e�X�gPOST�@������

	void Get(std::string path, std::string deststr);
	void Post(std::string postMassage, std::string contentType, std::string path, std::string deststr);

	std::string getStatusMassage();
	std::string getResult();
	std::string getResultJson();//json�`���ɐ��`���ꂽ�`�ŕԂ�
	std::string getFilePath();
};