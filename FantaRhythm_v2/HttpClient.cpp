#include "HttpClient.h"
#pragma warning( disable : 4996)
std::string deststr = "httpbin.org";//送信先(IPアドレスorドメイン名)
//const std::string deststr = "3.225.168.125";//送信先(IPアドレス)
const int port = 80;
HttpClient::HttpClient() {
	filepath = "test.json";
	statusMassage = "初期化成功";
	result = "";
	int err = WSAStartup(MAKEWORD(2, 0), &wsaData);//winsock初期化
	if (err != 0) {
		statusMassage = "初期化失敗";
		return;
	}
}
HttpClient::~HttpClient() {
	WSACleanup();//winsock終了
}
void HttpClient::testSend() {
	result.clear();
	try {
		sock = socket(AF_INET, SOCK_STREAM, 0);//ipv4 tcp指定
		if (sock == INVALID_SOCKET) {
			throw "ソケット作成失敗";
		}
		//サーバー側の設定
		server.sin_family = AF_INET;
		server.sin_port = htons(80);
		server.sin_addr.S_un.S_addr = inet_addr(deststr.c_str());
		if (server.sin_addr.S_un.S_addr == 0xFFFFFFFF) {//名前解決が必要な場合
			struct hostent* host;

			host = gethostbyname(deststr.c_str());
			if (host == NULL) {
				throw "名前解決失敗";
			}
			addrptr = (unsigned int**)host->h_addr_list;//アドレス取得

			while (*addrptr != NULL) {
				server.sin_addr.S_un.S_addr = *(*addrptr);
				//コネクション待機
				if (connect(sock, (struct sockaddr*) & server, sizeof(server)) == 0) {
					break;
				}
				addrptr++;//コネクション失敗したら次のアドレスで試行
			}
			//コネクションすべて失敗
			if (*addrptr == NULL) {
				throw "コネクション確立失敗";
			}
		}
		else {//アドレス直接指定の場合
			if (connect(sock, (struct sockaddr*) & server, sizeof(server)) != 0) {
				throw "コネクション確立失敗";
			}
		}

		//HTTPで/をリクエスト　テスト
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "GET /json HTTP/1.0\r\n\r\n");
		//HTTPリクエスト送信
		int n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "送信エラー";
		}
		//サーバーからHTTP受信
		while (n > 0) {
			memset(buf, 0, sizeof(buf));
			n = recv(sock, buf, sizeof(buf), 0);
			if (n < 0) {
				throw "受信エラー";
			}
			//受信結果を表示
			for (int i = 0; i < n; i++) {
				result.push_back(buf[i]);
			}
		}
	}
	catch (const char* err) {
		statusMassage = err;
	}
	closesocket(sock);
	//テスト用 Jsonで書き出し
	jsonWriter();
}

void HttpClient::jsonWriter() {
	std::ofstream outputfile(filepath);
	outputfile << getResultJsonString();
	outputfile.close();
}

void HttpClient::request(int chaNum[4]){
	result.clear();
	try {
		sock = socket(AF_INET, SOCK_STREAM, 0);//ipv4 tcp指定
		if (sock == INVALID_SOCKET) {
			throw "ソケット作成失敗";
		}
		//サーバー側の設定
		server.sin_family = AF_INET;
		server.sin_port = htons(80);
		server.sin_addr.S_un.S_addr = inet_addr(deststr.c_str());
		if (server.sin_addr.S_un.S_addr == 0xFFFFFFFF) {//名前解決が必要な場合
			struct hostent* host;

			host = gethostbyname(deststr.c_str());
			if (host == NULL) {
				throw "名前解決失敗";
			}
			addrptr = (unsigned int**)host->h_addr_list;//アドレス取得

			while (*addrptr != NULL) {
				server.sin_addr.S_un.S_addr = *(*addrptr);
				//コネクション待機
				if (connect(sock, (struct sockaddr*) & server, sizeof(server)) == 0) {
					break;
				}
				addrptr++;//コネクション失敗したら次のアドレスで試行
			}
			//コネクションすべて失敗
			if (*addrptr == NULL) {
				throw "コネクション確立失敗";
			}
		}
		else {//アドレス直接指定の場合
			if (connect(sock, (struct sockaddr*) & server, sizeof(server)) != 0) {
				throw "コネクション確立失敗";
			}
		}

		//HTTPで/をリクエスト　テスト
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "GET /json?cha1=%d&cha2=%d&cha3=%d&cha4=%d HTTP/1.0\r\n\r\n", chaNum[0], chaNum[1], chaNum[2], chaNum[3]);
		//HTTPリクエスト送信
		int n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "送信エラー";
		}
		//サーバーからHTTP受信
		while (n > 0) {
			memset(buf, 0, sizeof(buf));
			n = recv(sock, buf, sizeof(buf), 0);
			if (n < 0) {
				throw "受信エラー";
			}
			//受信結果を表示
			for (int i = 0; i < n; i++) {
				result.push_back(buf[i]);
			}
		}
	}
	catch (const char* err) {
		statusMassage = err;
	}
	closesocket(sock);
	//テスト用 Jsonで書き出し
	jsonWriter();
}

void HttpClient::testPost(std::string postMassage) {
	result.clear();
	try {
		sock = socket(AF_INET, SOCK_STREAM, 0);//ipv4 tcp指定
		if (sock == INVALID_SOCKET) {
			throw "ソケット作成失敗";
		}
		//サーバー側の設定
		server.sin_family = AF_INET;
		server.sin_port = htons(port);
		server.sin_addr.S_un.S_addr = inet_addr(deststr.c_str());
		if (server.sin_addr.S_un.S_addr == 0xFFFFFFFF) {//名前解決が必要な場合
			struct hostent* host;

			host = gethostbyname(deststr.c_str());
			if (host == NULL) {
				throw "名前解決失敗";
			}
			addrptr = (unsigned int**)host->h_addr_list;//アドレス取得

			while (*addrptr != NULL) {
				server.sin_addr.S_un.S_addr = *(*addrptr);
				//コネクション待機
				if (connect(sock, (struct sockaddr*) & server, sizeof(server)) == 0) {
					break;
				}
				addrptr++;//コネクション失敗したら次のアドレスで試行
			}
			//コネクションすべて失敗
			if (*addrptr == NULL) {
				throw "コネクション確立失敗";
			}
		}
		else {//アドレス直接指定の場合
			if (connect(sock, (struct sockaddr*) & server, sizeof(server)) != 0) {
				throw "コネクション確立失敗";
			}
		}

		//HTTPで/をリクエスト　テスト
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "POST /post HTTP/1.0\r\n");
		int n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "送信エラー";
		}
		_snprintf(buf, sizeof(buf), "Host: %s:%d\r\n",deststr.c_str() , port);
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "送信エラー";
		}
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "Content-Length: %d\r\n", (int)sizeof(postMassage.c_str()));
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "送信エラー";
		}
		_snprintf(buf, sizeof(buf), "\r\n");
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "送信エラー";
		}
		send(sock, postMassage.c_str(), sizeof(postMassage.c_str()), 0);




		//サーバーからHTTP受信
		while (n > 0) {
			memset(buf, 0, sizeof(buf));
			n = recv(sock, buf, sizeof(buf), 0);
			if (n < 0) {
				throw "受信エラー";
			}
			//受信結果を表示
			for (int i = 0; i < n; i++) {
				result.push_back(buf[i]);
			}
		}
	}
	catch (const char* err) {
		statusMassage = err;
	}
	closesocket(sock);
	//テスト用 Jsonで書き出し
	jsonWriter();
}

std::string HttpClient::getStatusMassage() {
	return statusMassage;
}

std::string HttpClient::getResult() {
	return result;
}

std::string HttpClient::getResultJsonString() {//json形式に整形された形で返す
	if (result.size() > 0) {
		return result.substr(result.find("{"));
	}
	return "";
}

std::string HttpClient::getFilePath() {
	return filepath;
}

