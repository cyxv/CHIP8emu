#include "input.h"

#include "../include/raylib/raylib.h"

// this sucks
void Input::update() {
	if (IsWindowFocused()) {
		active_keys[0x1] = IsKeyDown(KEY_ONE);
		active_keys[0x2] = IsKeyDown(KEY_TWO);
		active_keys[0x3] = IsKeyDown(KEY_THREE);
		active_keys[0xC] = IsKeyDown(KEY_FOUR);
		active_keys[0x4] = IsKeyDown(KEY_Q);
		active_keys[0x5] = IsKeyDown(KEY_W);
		active_keys[0x6] = IsKeyDown(KEY_E);
		active_keys[0xD] = IsKeyDown(KEY_R);
		active_keys[0x7] = IsKeyDown(KEY_A);
		active_keys[0x8] = IsKeyDown(KEY_S);
		active_keys[0x9] = IsKeyDown(KEY_D);
		active_keys[0xE] = IsKeyDown(KEY_F);
		active_keys[0xA] = IsKeyDown(KEY_Z);
		active_keys[0x0] = IsKeyDown(KEY_X);
		active_keys[0xB] = IsKeyDown(KEY_C);
		active_keys[0xF] = IsKeyDown(KEY_V);
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