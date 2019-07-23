#include "HitPoint.h"

HitPoint::HitPoint() {
	hp = 100;
}
HitPoint::~HitPoint() {

}

void HitPoint::funcEvent(Obj o) {
	switch (o.msg) {
	case Massage::SMALLDAMAGE:
		hp--;
		break;
	}
}

void HitPoint::update() {

}
void HitPoint::draw() {
	Print << U"HitPoint = "<< hp;
}