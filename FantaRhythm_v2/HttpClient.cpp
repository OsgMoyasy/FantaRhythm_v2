#include "HttpClient.h"
#pragma warning( disable : 4996)
//std::string deststr = "httpbin.org";//���M��(IP�A�h���Xor�h���C����)
//std::string deststr = "postman-echo.com";//���M��(IP�A�h���X)
const int port = 3000;
HttpClient::HttpClient() {
	filepath = "chardata.json";
	statusMassage = "����������";
	result = "";
	int err = WSAStartup(MAKEWORD(2, 0), &wsaData);//winsock������
	if (err != 0) {
		statusMassage = "���������s";
		return;
	}
}
HttpClient::~HttpClient() {
	WSACleanup();//winsock�I��
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
		sock = socket(AF_INET, SOCK_STREAM, 0);//ipv4 tcp�w��
		if (sock == INVALID_SOCKET) {
			throw "�\�P�b�g�쐬���s";
		}
		//�T�[�o�[���̐ݒ�
		server.sin_family = AF_INET;
		server.sin_port = htons(port);
		server.sin_addr.S_un.S_addr = inet_addr(deststr.c_str());
		if (server.sin_addr.S_un.S_addr == 0xFFFFFFFF) {//���O�������K�v�ȏꍇ
			struct hostent* host;

			host = gethostbyname(deststr.c_str());
			if (host == NULL) {
				throw "���O�������s";
			}
			addrptr = (unsigned int**)host->h_addr_list;//�A�h���X�擾

			while (*addrptr != NULL) {
				server.sin_addr.S_un.S_addr = *(*addrptr);
				//�R�l�N�V�����ҋ@
				if (connect(sock, (struct sockaddr*) & server, sizeof(server)) == 0) {
					break;
				}
				addrptr++;//�R�l�N�V�������s�����玟�̃A�h���X�Ŏ��s
			}
			//�R�l�N�V�������ׂĎ��s
			if (*addrptr == NULL) {
				throw "�R�l�N�V�����m�����s";
			}
		}
		else {//�A�h���X���ڎw��̏ꍇ
			if (connect(sock, (struct sockaddr*) & server, sizeof(server)) != 0) {
				throw "�R�l�N�V�����m�����s";
			}
		}

		//HTTP��/�����N�G�X�g�@�e�X�g
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "GET %s HTTP/1.0\r\n", path.c_str());
		//HTTP���N�G�X�g���M
		int n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "���M�G���[";
		}
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "Host: %s\r\n", deststr.c_str());
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "���M�G���[";
		}
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "\r\n");
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "���M�G���[";
		}
		//�T�[�o�[����HTTP��M
		while (n > 0) {
			memset(buf, 0, sizeof(buf));
			n = recv(sock, buf, sizeof(buf), 0);
			if (n < 0) {
				throw "��M�G���[";
			}
			//��M���ʂ�\��
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
		sock = socket(AF_INET, SOCK_STREAM, 0);//ipv4 tcp�w��
		if (sock == INVALID_SOCKET) {
			throw "�\�P�b�g�쐬���s";
		}
		//�T�[�o�[���̐ݒ�
		server.sin_family = AF_INET;
		server.sin_port = htons(port);
		server.sin_addr.S_un.S_addr = inet_addr(deststr.c_str());
		if (server.sin_addr.S_un.S_addr == 0xFFFFFFFF) {//���O�������K�v�ȏꍇ
			struct hostent* host;

			host = gethostbyname(deststr.c_str());
			if (host == NULL) {
				throw "���O�������s";
			}
			addrptr = (unsigned int**)host->h_addr_list;//�A�h���X�擾

			while (*addrptr != NULL) {
				server.sin_addr.S_un.S_addr = *(*addrptr);
				//�R�l�N�V�����ҋ@
				if (connect(sock, (struct sockaddr*) & server, sizeof(server)) == 0) {
					break;
				}
				addrptr++;//�R�l�N�V�������s�����玟�̃A�h���X�Ŏ��s
			}
			//�R�l�N�V�������ׂĎ��s
			if (*addrptr == NULL) {
				throw "�R�l�N�V�����m�����s";
			}
		}
		else {//�A�h���X���ڎw��̏ꍇ
			if (connect(sock, (struct sockaddr*) & server, sizeof(server)) != 0) {
				throw "�R�l�N�V�����m�����s";
			}
		}

		//HTTP��/�����N�G�X�g�@�e�X�g
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "POST %s HTTP/1.0\r\n", path.c_str());
		int n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "���M�G���[";
		}
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "Host: %s\r\n", deststr.c_str());
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "���M�G���[";
		}
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "Content-Length: %d\r\n", postMassage.size());
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "���M�G���[";
		}
		_snprintf(buf, sizeof(buf), "\r\n");
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "���M�G���[";
		}
		send(sock, postMassage.c_str(), postMassage.size(), 0);

		//�T�[�o�[����HTTP��M
		while (n > 0) {
			memset(buf, 0, sizeof(buf));
			n = recv(sock, buf, sizeof(buf), 0);
			if (n < 0) {
				throw "��M�G���[";
			}
			//��M���ʂ�\��
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

std::string HttpClient::getResultJson() {//json�`���ɐ��`���ꂽ�`�ŕԂ�
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