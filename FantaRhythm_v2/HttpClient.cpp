#include "HttpClient.h"
#pragma warning( disable : 4996)
std::string deststr = "httpbin.org";//���M��(IP�A�h���Xor�h���C����)
//std::string deststr = "postman-echo.com";//���M��(IP�A�h���X)
const int port = 3000;
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
	//�e�X�g�p Json�ŏ����o��
	jsonWriter();
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

void HttpClient::Post(std::string postMassage, std::string contentType, std::string path, std::string deststr) {
	postMassage = percentEnc(postMassage);
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
		/*
		memset(buf, 0, sizeof(buf));
		_snprintf(buf, sizeof(buf), "Content-Type: %s\r\n", contentType.c_str());
		n = send(sock, buf, (int)strlen(buf), 0);
		if (n < 0) {
			throw "���M�G���[";
		}
		*/
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

std::string HttpClient::percentEnc(std::string str) {
	const int NUM_BEGIN_UTF8 = 48;
	const int CAPITAL_BEGIN_UTF8 = 65;
	const int LOWER_BEGIN_UTF8 = 97;

	int charCode = -1;
	std::string encoded;
	std::stringstream out;

	//for����1byte���X�g���[���ɓ���Ă���
	for (int i = 0; str[i] != 0; i++) {
		//�����񒆂�1byte���̃f�[�^�𐮐��l�Ƃ��đ��
		charCode = (int)(unsigned char)str[i];

		//�G���R�[�h����K�v�̖��������̔���
		if ((NUM_BEGIN_UTF8 <= charCode && charCode <= NUM_BEGIN_UTF8 + 9)
			|| (CAPITAL_BEGIN_UTF8 <= charCode && charCode <= CAPITAL_BEGIN_UTF8 + 25)
			|| (LOWER_BEGIN_UTF8 <= charCode && charCode <= LOWER_BEGIN_UTF8 + 25)
			|| str[i] == '.' || str[i] == '_' || str[i] == '-' || str[i] == '~')
		{
			//�G���R�[�h�̕K�v�����������͂��̂܂܃X�g���[���ɓ����
			out << str[i];
		}
		else {
			//�G���R�[�h����ꍇ��%�L���ƕ����R�[�h��16�i���\�����X�g���[���ɓ����
			out << '%' << std::hex << std::uppercase << charCode;
		}
	}
	//�X�g���[���̕������string�̃C���X�^���X�ɑ����return
	encoded = out.str();
	return encoded;
}