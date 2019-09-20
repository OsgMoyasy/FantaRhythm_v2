#pragma once

namespace JUDGE {
	enum TYPE {
		PERFECT,
		GREAT,
		GOOD,
		BAD,
		NONE,
		TYPE_SIZE,
	};

	typedef struct _JudgeCount {//判定のカウント数を格納していく
		int cnt[TYPE::TYPE_SIZE];
	}JudgeCount;
}

