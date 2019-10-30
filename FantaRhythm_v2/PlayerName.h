#pragma once
#include <Siv3D.hpp>

class PlayerName {
private:
	static String pName;
public:
	static void setName(String name);
	static String getName();
};