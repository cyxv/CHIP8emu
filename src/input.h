#pragma once

#include <array>

class Input {
public:
	void update();
	bool is_key_active(unsigned char key);
	std::array<bool, 16> active_keys{};
};