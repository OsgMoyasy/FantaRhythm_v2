#pragma once
#include <Windows.h>
#include <string>

std::string sSjistoUTF8(std::string srcSjis) {
	//Unicode�֕ϊ���̕�����
	int lengthUnicode = MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, NULL, 0);
	//�K�v�ȕ�����Unicode������̃o�b�t�@
	wchar_t* bufUnicode = new wchar_t[lengthUnicode];
	//ShiftJis����Unicode�ϊ�
	MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, bufUnicode, lengthUnicode);
	//UTF8�֕ϊ���̕�����
	int length UTF8 = new char[lengthUTF8];

	//�K�v�ȕ�����UTF8�̕�����
	char* bufUTF8;

}