#pragma once

#include <iostream>
#include <thread>
#include <vector>

#include "../include/raylib/raylib.h"

class Display {
public:
	Display();
	void begin();
	void draw();
	void end();
	bool update_screen();
	bool clear_screen();

	bool set_pixel(int x, int y, bool state);
	bool get_pixel(int x, int y);

	bool to_update = false;
private:
	std::vector<std::vector<bool>> pixel_states;
	bool fill_pixel(int x, int y);
};