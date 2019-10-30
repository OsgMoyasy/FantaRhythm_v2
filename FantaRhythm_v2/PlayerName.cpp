#include "PlayerName.h"

String PlayerName::pName;

void PlayerName::setName(String name) {
	pName = name;
}
String PlayerName::getName() {
	return pName;
}