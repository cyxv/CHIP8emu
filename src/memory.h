#pragma once

#include <algorithm>
#include <array>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <span>
#include <vector>

class Memory {
public:
	unsigned char get(int address);
	std::vector<unsigned char> get(int address, size_t length);

	bool set(int address, unsigned char value);
	bool set(int address, std::vector<unsigned char> values);

	bool load_rom(std::string path);

	bool clear();

	Memory();
private:
	std::array<unsigned char, 0x1000> RAM;
};