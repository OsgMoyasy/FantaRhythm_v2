#include "HttpClient.h"
#pragma warning( disable : 4996)
std::string deststr = "httpbin.org";//送信先(IPアドレスorドメイン名)
//std::string deststr = "postman-echo.com";//送信先(IPアドレス)
const int port = 3000;
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


void HttpClient::jsonWriter() {
	std::ofstream outputfile(filepath);
	outputfile << getResultJson();
	outputfile.close();
}
/*
void HttpClient::characterDataRequest(int chaNum[4]){
	std::stringstream st;
	st << "/json?cha1=" << chaNum[0] << "&cha2=" << chaNum[1] << "&cha3=" << chaNum[2] << "&cha4=" << chaNum[3];
	Get(st.str(), deststr);
	jsonWriter();
}
*/
void HttpClient::testPost(std::string postMassage) {
	Post(postMassage, "Content - Type: application / json; charset = utf8", "/post", deststr);
	//テスト用 Jsonで書き出し
	jsonWriter();
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

void HttpClient::Post(std::string postMassage, std::string contentType, std::string path, std::string deststr) {
	postMassage = percentEnc(postMassage);
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
		/*
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "Content-Type: %s\r\n", contentType.c_str());
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "送信エラー";
		}
		*/
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

std::string HttpClient::percentEnc(std::string str) {
	const int NUM_BEGIN_UTF8 = 48;
	const int CAPITAL_BEGIN_UTF8 = 65;
	const int LOWER_BEGIN_UTF8 = 97;

	int charCode = -1;
	std::string encoded;
	std::stringstream out;

	//for文で1byteずつストリームに入れていく
	for (int i = 0; str[i] != 0; i++) {
		//文字列中の1byte分のデータを整数値として代入
		charCode = (int)(unsigned char)str[i];

		//エンコードする必要の無い文字の判定
		if ((NUM_BEGIN_UTF8 <= charCode && charCode <= NUM_BEGIN_UTF8 + 9)
			|| (CAPITAL_BEGIN_UTF8 <= charCode && charCode <= CAPITAL_BEGIN_UTF8 + 25)
			|| (LOWER_BEGIN_UTF8 <= charCode && charCode <= LOWER_BEGIN_UTF8 + 25)
			|| str[i] == '.' || str[i] == '_' || str[i] == '-' || str[i] == '~')
		{
			//エンコードの必要が無い文字はそのままストリームに入れる
			out << str[i];
		}
		else {
			//エンコードする場合は%記号と文字コードの16進数表示をストリームに入れる
			out << '%' << std::hex << std::uppercase << charCode;
		}
	}
	//ストリームの文字列をstringのインスタンスに代入しreturn
	encoded = out.str();
	return encoded;
}