#pragma once

namespace JUDGE {
	enum TYPE {
		PERFECT,
		GREAT,
		GOOD,
		BAD,
		TYPE_SIZE,
		NONE,
	};

	typedef struct _JudgeCount {//����̃J�E���g�����i�[���Ă���
		int cnt[TYPE::TYPE_SIZE];
		int maxcombo;
	}JudgeCount;
}

