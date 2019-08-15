#include "Result.h"


Result::Result(JUDGE::JudgeCount* judgecnt, int totaldmg, bool cflag) {
	judgecount = judgecnt;
	totaldamage = totaldmg;
	clearflag = cflag;
}
Result::~Result(void) {

}
void Result::update(void) {

}
void Result::draw(void) {

}