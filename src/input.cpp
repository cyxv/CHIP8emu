#include "input.h"

#include "../include/raylib/raylib.h"

// this sucks
void Input::update() {
	if (IsWindowFocused()) {
		active_keys[0x1] = IsKeyDown(KEY_ONE); // 1
		active_keys[0x2] = IsKeyDown(KEY_TWO); // 2
		active_keys[0x3] = IsKeyDown(KEY_THREE); // 3
		active_keys[0xC] = IsKeyDown(KEY_FOUR); // 4
		active_keys[0x4] = IsKeyDown(KEY_Q); // Q
		active_keys[0x5] = IsKeyDown(KEY_W); // W
		active_keys[0x6] = IsKeyDown(KEY_E); // E
		active_keys[0xD] = IsKeyDown(KEY_R); // R
		active_keys[0x7] = IsKeyDown(KEY_A); // A
		active_keys[0x8] = IsKeyDown(KEY_S); // S
		active_keys[0x9] = IsKeyDown(KEY_D); // D
		active_keys[0xE] = IsKeyDown(KEY_F); // F
		active_keys[0xA] = IsKeyDown(KEY_Z); // Z
		active_keys[0x0] = IsKeyDown(KEY_X); // X
		active_keys[0xB] = IsKeyDown(KEY_C); // C
		active_keys[0xF] = IsKeyDown(KEY_V); // V
	}
	else {
		for (int i = 0; i < 16; i++) {
			active_keys[i] = false;
		}
	}
}

bool Input::is_key_active(unsigned char key) {
	return active_keys[key];
}