#pragma once

namespace JUDGE {
	enum TYPE {
		PERFECT,
		GREAT,
		GOOD,
		BAD,
		TYPE_SIZE,
	};

	typedef struct _JudgeCount {//”»’è‚ÌƒJƒEƒ“ƒg”‚ğŠi”[‚µ‚Ä‚¢‚­
		int cnt[TYPE::TYPE_SIZE];
	}JudgeCount;
}

