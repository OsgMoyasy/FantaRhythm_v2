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

	typedef struct _JudgeCount {//����̃J�E���g�����i�[���Ă���
		int cnt[TYPE::TYPE_SIZE];
	}JudgeCount;
}

