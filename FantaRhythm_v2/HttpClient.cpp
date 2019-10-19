#include "HttpClient.h"
#pragma warning( disable : 4996)
std::string deststr = "httpbin.org";//���M��(IP�A�h���Xor�h���C����)
//const std::string deststr = "3.225.168.125";//���M��(IP�A�h���X)
const int port = 80;
HttpClient::HttpClient() {
	filepath = "test.json";
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
void HttpClient::testSend() {
	result.clear();
	try {
		sock = socket(AF_INET, SOCK_STREAM, 0);//ipv4 tcp�w��
		if (sock == INVALID_SOCKET) {
			throw "�\�P�b�g�쐬���s";
		}
		//�T�[�o�[���̐ݒ�
		server.sin_family = AF_INET;
		server.sin_port = htons(80);
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
		_snprintf(buf, sizeof(buf), "GET /json HTTP/1.0\r\n\r\n");
		//HTTP���N�G�X�g���M
		int n = send(sock, buf, (int)strlen(buf), 0);
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
	//�e�X�g�p Json�ŏ����o��
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
		sock = socket(AF_INET, SOCK_STREAM, 0);//ipv4 tcp�w��
		if (sock == INVALID_SOCKET) {
			throw "�\�P�b�g�쐬���s";
		}
		//�T�[�o�[���̐ݒ�
		server.sin_family = AF_INET;
		server.sin_port = htons(80);
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
		_snprintf(buf, sizeof(buf), "GET /json?cha1=%d&cha2=%d&cha3=%d&cha4=%d HTTP/1.0\r\n\r\n", chaNum[0], chaNum[1], chaNum[2], chaNum[3]);
		//HTTP���N�G�X�g���M
		int n = send(sock, buf, (int)strlen(buf), 0);
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
	//�e�X�g�p Json�ŏ����o��
	jsonWriter();
}

void HttpClient::testPost(std::string postMassage) {
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
		_snprintf(buf, sizeof(buf), "POST /post HTTP/1.0\r\n");
		int n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "���M�G���[";
		}
		_snprintf(buf, sizeof(buf), "Host: %s:%d\r\n",deststr.c_str() , port);
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "���M�G���[";
		}
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "Content-Length: %d\r\n", (int)sizeof(postMassage.c_str()));
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "���M�G���[";
		}
		_snprintf(buf, sizeof(buf), "\r\n");
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "���M�G���[";
		}
		send(sock, postMassage.c_str(), sizeof(postMassage.c_str()), 0);




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
	//�e�X�g�p Json�ŏ����o��
	jsonWriter();
}

std::string HttpClient::getStatusMassage() {
	return statusMassage;
}

std::string HttpClient::getResult() {
	return result;
}

std::string HttpClient::getResultJsonString() {//json�`���ɐ��`���ꂽ�`�ŕԂ�
	if (result.size() > 0) {
		return result.substr(result.find("{"));
	}
	return "";
}

std::string HttpClient::getFilePath() {
	return filepath;
}

