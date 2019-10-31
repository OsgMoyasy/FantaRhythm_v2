#include "HttpClient.h"
#pragma warning( disable : 4996)
//std::string deststr = "httpbin.org";//送信先(IPアドレスorドメイン名)
//std::string deststr = "postman-echo.com";//送信先(IPアドレス)
const int port = 3000;
HttpClient::HttpClient() {
	filepath = "chardata.json";
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


void HttpClient::jsonWriter() {
	std::ofstream outputfile(filepath);
	outputfile << getResultJson();
	outputfile.close();
}


void HttpClient::Get(std::string path, std::string deststr, TH_STATUS& isFinish) {
	isFinish = TH_ACTIVE;
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
		_snprintf(buf, sizeof(buf), "GET %s HTTP/1.0\r\n", path.c_str());
		//HTTPリクエスト送信
		int n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "送信エラー";
		}
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "Host: %s\r\n", deststr.c_str());
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "送信エラー";
		}
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "\r\n");
		n = send(sock, buf, (int)strlen(buf), 0);
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
	jsonWriter();
	isFinish = TH_FINISH;
}

void HttpClient::Post(std::string post, std::string path, std::string deststr, TH_STATUS& isFinish) {
	std::string postMassage = encode(post);
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
		_snprintf(buf, sizeof(buf), "POST %s HTTP/1.0\r\n", path.c_str());
		int n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "送信エラー";
		}
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "Host: %s\r\n", deststr.c_str());
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "送信エラー";
		}
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "Content-Length: %d\r\n", postMassage.size());
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "送信エラー";
		}
		_snprintf(buf, sizeof(buf), "\r\n");
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "送信エラー";
		}
		send(sock, postMassage.c_str(), postMassage.size(), 0);

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
}

std::string HttpClient::getStatusMassage() {
	return statusMassage;
}

std::string HttpClient::getResult() {
	return result;
}

std::string HttpClient::getResultJson() {//json形式に整形された形で返す
	if (result.size() > 0) {
		return result.substr(result.find("{"));
	}
	return "";
}

std::string HttpClient::getFilePath() {
	return filepath;
}


std::string HttpClient::encode(const std::string& str) {
	std::ostringstream os;

	for (int i = 0; i < str.size(); i++) {
		char c = str[i];
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
			(c >= '0' && c <= '9') ||
			c == '-' || c == '_' || c == '.' || c == '~'|| c == '=' || c == '&') {
			os << c;
		}
		else {
			char s[4];
			snprintf(s, sizeof(s), "%%%02x", c & 0xff);
			os << s;
		}
	}

	return os.str();
}