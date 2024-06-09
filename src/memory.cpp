#include "memory.h"
#include "config.h"

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

	set(0x50, config::font); // set 050 - 09F to the font
}