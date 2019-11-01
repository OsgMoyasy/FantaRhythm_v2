#include "RankingData.h"

String RankingData::pName;
String RankingData::user_id;
int RankingData::char_id[4];
String RankingData::music_name;

void RankingData::setName(String name) {
	pName = name;
}
String RankingData::getName() {
	return pName;
}

void RankingData::setUser_id(String id) {
	user_id = id;
}
void RankingData::setChar_id(int index, int id) {
	char_id[index] = id;
}

String RankingData::getUser_id() {
	return user_id;
}
void RankingData::getChar_id(int id[4]) {
	for (int i = 0; i < 4; i++) {
		id[i] = char_id[i];
	}
}

String RankingData::getMusic_name() {
	return music_name;
}
void RankingData::setMusic_name(String name) {
	music_name = name;
}