#include "display.h"
#include "config.h"

Display::Display() {
	std::vector<bool> default_row;
	for (int i = 0; i < 32; i++) {
		default_row.push_back(false);
	}
	for (int i = 0; i < 64; i++) {
		pixel_states.push_back(default_row);
	}
}

void Display::begin() {
	InitWindow(64 * config::pixel_size, 32 * config::pixel_size, "CHIP8emu");
	SetTargetFPS(60);
	ClearBackground({ 0, 0, 0, 255 });
}

void Display::draw() {
	BeginDrawing();
	if (to_update) {
		update_screen();
		to_update = false;
	}
	EndDrawing();
	SwapScreenBuffer();
}

void Display::end() {
	CloseWindow();
}

bool Display::fill_pixel(int x, int y) {
	int relative_x = x * config::pixel_size, relative_y = y * config::pixel_size;

	DrawRectangle(relative_x, relative_y, config::pixel_size, config::pixel_size, { 255, 255, 255, 255 });
	return true;
}

bool Display::set_pixel(int x, int y, bool state) {
	pixel_states[x][y] = state;
	return true;
}

bool Display::get_pixel(int x, int y) {
	return pixel_states[x][y];
}

bool Display::update_screen() {
	ClearBackground({ 0, 0, 0, 255 });
	for (int x = 0; x < pixel_states.size(); x++) {
		for (int y = 0; y < pixel_states[y].size(); y++) {
			if (pixel_states[x][y]) {
				fill_pixel(x, y);
			}
		}
	}
	return true;
}

bool Display::clear_screen() {
	for (int x = 0; x < pixel_states.size(); x++) {
		for (int y = 0; y < pixel_states[y].size(); y++) {
			pixel_states[x][y] = false;
		}
	}
	return true;
}