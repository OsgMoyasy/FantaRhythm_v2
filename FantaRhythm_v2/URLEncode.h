#pragma once
#include <Windows.h>
#include <string>

std::string sSjistoUTF8(std::string srcSjis) {
	//Unicodeへ変換後の文字列長
	int lengthUnicode = MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, NULL, 0);
	//必要な分だけUnicode文字列のバッファ
	wchar_t* bufUnicode = new wchar_t[lengthUnicode];
	//ShiftJisからUnicode変換
	MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, bufUnicode, lengthUnicode);
	//UTF8へ変換後の文字列長
	int length UTF8 = new char[lengthUTF8];

	//必要な分だけUTF8の文字列
	char* bufUTF8;

}