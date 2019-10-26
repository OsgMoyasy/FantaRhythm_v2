#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <WS2tcpip.h>
#include<fstream>
#include <sstream>

enum STATUS {//スレッド用現在の状態
	S_NONE,
	S_ERR,
	S_ACTIVE,
	S_FINISH,
};

class HttpClient {
private:
	WSADATA wsaData;
	struct sockaddr_in server;
	SOCKET sock;
	char buf[1024];
	unsigned int** addrptr;
	STATUS status;

	std::string statusMassage;
	std::string result;

	std::string filepath;

	std::string getStatusMassage();
	std::string getResult();
	std::string getResultJson();//json形式に整形された形で返す
	std::string getFilePath();
	

public:
	HttpClient();
	~HttpClient();
	void testGet();
	//void characterDataRequest(int chaNum[4]);

	void testPost(std::string postMassage);//テストPOST　未完成
	void jsonWriter();
	void Get(std::string path, std::string deststr);
	void Post(std::string postMassage, std::string contentType, std::string path, std::string deststr);
	
	STATUS getStatus();

	
};