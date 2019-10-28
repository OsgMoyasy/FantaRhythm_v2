#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <WS2tcpip.h>
#include<fstream>
#include <sstream>
#include "MyThread.h"
#include <iomanip>


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

	std::string getStatusMassage();
	std::string getResult();
	std::string getResultJson();//json形式に整形された形で返す
	std::string getFilePath();

public:
	HttpClient();
	~HttpClient();
	//void characterDataRequest(int chaNum[4]);

	void testPost(std::string postMassage);//テストPOST　未完成
	void jsonWriter();
	void Get(std::string path, std::string deststr, TH_STATUS& isFinish);//スレッドに対応
	void Post(std::string postMassage, std::string contentType, std::string path, std::string deststr);	
	std::string percentEnc(std::string str);
};