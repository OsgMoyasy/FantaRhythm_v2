#pragma once
#include <Siv3D.hpp>

class RankingData {
private:
	static String pName;
	static String user_id;
	static int char_id[4];
	static String music_name;
public:
	static void setName(String name);
	static void setUser_id(String id);
	static void setChar_id(int index, int id);
	static String getName();
	static String getUser_id();
	static void getChar_id(int id[4]);
	static String getMusic_name();
	static void setMusic_name(String name);

};