#pragma once
#include <cstdint>
#include"ImGuiManager.h"
class Score {
public:
	void Initialize();

	void Update();

	static const int Timer = 60;

	private:

	int32_t timer_;

	int32_t score_;

};
