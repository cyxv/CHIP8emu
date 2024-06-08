#include "memory.h"

unsigned char Memory::get(int address) {
	if (address > 0x1000) {
		throw std::out_of_range(std::format("Attempt to get out of bounds @ 0x{:x}\n", address));
	}
	return RAM[address];
}

std::vector<unsigned char> Memory::get(int address, size_t length) {
	if (address > 0x1000) {
		throw std::out_of_range(std::format("Attempt to get out of bounds @ 0x{:x}\n", address));
	}

	std::vector<unsigned char> out{};

	auto slice = std::span{ RAM.cbegin() + address, length };
	out.assign(slice.begin(), slice.end());
	return out;
}

bool Memory::set(int address, unsigned char value) {
	try {
		if (address > 0x1000) {
			std::cout << "Attempt to set out of bounds @ 0x" << std::hex << std::format("{}\n", address);
			return false;
		}
		RAM[address] = value;
	}
	catch (std::exception e) {
		std::cout << std::format("Error occured in set() [{}]\n", e.what());
		return false;
	}
	return true;
}

bool Memory::set(int address, std::vector<unsigned char> values) {
	try {
		for (int i = 0; i < values.size(); i++) {
			if (address + i > 0x1000) {
				std::cout << "Attempt to set out of bounds @ 0x" << std::hex << std::format("{}\n", address + i);
				return false;
			}
			RAM[static_cast<size_t>(address) + i] = values[i];
		}
	}
	catch (std::exception e) {
		std::cout << std::format("Error occured in set() [{}]\n", e.what());
		return false;
	}
	return true;
}

bool Memory::load_rom(std::string path) {
	std::ifstream f(path, std::ios::binary);
	std::vector<unsigned char> rom = std::vector<unsigned char>(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
	f.close();

	set(0x200, rom);
	return true;
}

bool Memory::clear() {
	try {
		std::fill_n(RAM.begin(), RAM.size(), 0x00);
	}
	catch (std::exception e) {
		std::cout << std::format("Error occured in clear() [{}]\n", e.what());
		return false;
	}
	return true;
}

// on memory initialisation
Memory::Memory() {
	clear();

	std::vector<unsigned char> font{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0 (50-54)
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
	set(0x50, font); // set 050 - 09F to the font
}