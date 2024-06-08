#pragma once

#include "config.h"
#include "memory.h"
#include "display.h"
#include "input.h"

#include <random>

class CPU {
public:
	CPU(Memory* memory, Display* display, Input* input);
	void cycle(int times);

	unsigned char delay_timer{ 0 };
	unsigned char sound_timer{ 0 };
private:
	// registers & stack
	std::array<unsigned char, 16> V;
	unsigned short I, PC;
	std::vector<unsigned short> stack;

	// utility
	bool get_bit(unsigned char byte, unsigned char index);

	// cpu cycle
	unsigned short fetch();
	bool decode_execute(unsigned short instruction);

	// rng (for 1 cpu instruction)
	std::random_device rd;
	std::mt19937 gen{ rd() };
	std::uniform_int_distribution<int> rng{ 0, 255 };

	Memory* _memory;
	Display* _display;
	Input* _input;
};