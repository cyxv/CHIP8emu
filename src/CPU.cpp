#include "CPU.h"

// 0xD

CPU::CPU(Memory* memory, Display* display, Input* input) {
	_memory = memory;
	_display = display;
	_input = input;

	// initialise registers & program counter
	for (int i = 0; i < 15; i++) {
		V[i] = 0;
	}
	I = 0;
	PC = 0x200; // program starts at 0x200
}

bool CPU::get_bit(unsigned char byte, unsigned char index) {
	return (byte >> index) & 1;
}

unsigned short CPU::fetch() {
	std::vector<unsigned char> op = _memory->get(PC, 2);
	PC += 2;

	// turns two chars into short (some bitwise magic i barely understand currently)
	unsigned short out = (static_cast<unsigned short>(op[0]) << 8) | (op[1] & 0xFF);
	return out;
}

bool CPU::decode_execute(unsigned short instruction) {
	// important values
	unsigned char opcode = (instruction >> 12) & 0xF;
	unsigned char X = (instruction >> 8) & 0xF;
	unsigned char Y = (instruction >> 4) & 0xF;
	unsigned char N = instruction & 0xF;
	unsigned char NN = instruction & 0xFF;
	unsigned short NNN = instruction & 0x0FFF;

	/*
	std::cout << "instruction: " << std::hex << std::uppercase << instruction << "\n";
	std::cout << "opcode: 0x" << static_cast<int>(opcode) << "\n";
	std::cout << "X: 0x" << static_cast<int>(X) << "\n";
	std::cout << "Y: 0x" << static_cast<int>(Y) << "\n";
	std::cout << "N: 0x" << static_cast<int>(N) << "\n";
	std::cout << "NN: 0x" << static_cast<int>(NN) << "\n";
	std::cout << "NNN: 0x" << NNN << "\n";
	*/

	// execute CPU instruction
	switch (opcode) {
	case 0:
		//if (NNN == 0x0E0) {
		switch (NNN) {
		case 0xE0:
			_display->clear_screen();
			_display->to_update = true;
			break;
		case 0xEE:
			PC = stack.back();
			stack.pop_back();
		}
		break;
	case 1:
		PC = NNN;
		break;
	case 2:
		stack.push_back(PC);
		PC = NNN;
		break;
	case 3:
		if (V[X] == NN) {
			PC += 2;
		}
		break;
	case 4:
		if (V[X] != NN) {
			PC += 2;
		}
		break;
	case 5:
		if (N == 0 && V[X] == V[Y]) {
			PC += 2;
		}
		break;
	case 6:
		V[X] = NN;
		break;
	case 7:
		V[X] += NN;
		break;
	case 8:
		switch (N) {
		case 0:
			V[X] = V[Y];
			break;
		case 1:
			V[X] = V[X] | V[Y];
			break;
		case 2:
			V[X] = V[X] & V[Y];
			break;
		case 3:
			V[X] = V[X] ^ V[Y];
			break;
		case 4:
			V[0xF] = (V[X] + V[Y] > 255);
			V[X] += V[Y];
			break;
		case 5:
			V[X] -= V[Y];
			break;
		case 6:
			if (config::shift_vx_equals_vy) {
				V[X] = V[Y];
			}
			V[0xF] = (V[X] & 0xF);
			V[X] >>= 1;
			break;
		case 7:
			V[X] = V[Y] - V[X];
			break;
		case 0xE:
			if (config::shift_vx_equals_vy) {
				V[X] = V[Y];
			}
			V[0xF] = (V[X] >> 7) & 0xF;
			V[X] <<= 1;
			break;
		}
		break;
	case 9:
		if (N == 0 && V[X] != V[Y]) {
			PC += 2;
		}
		break;
	case 0xA:
		I = NNN;
		break;
	case 0xB:
		PC = NNN + ((config::add_vx_to_offset_jmp) ? V[0] : V[X]);
		break;
	case 0xC:
		V[X] = rng(gen) & NN;
		break;
	case 0xD: {
		int x = V[X] % 64;
		int y = V[Y] % 32;
		V[0xF] = 0;

		unsigned short base_sprite_address = I;

		for (int row = 0; row < N; row++) {
			unsigned char byte = _memory->get(I + row);

			for (int bit_index = 0; bit_index < 8; bit_index++) {
				if (x + bit_index > 64) {
					break;
				}
				bool current_pixel = get_bit(byte, 7 - bit_index);
				if (current_pixel) {
					if (_display->get_pixel(x + bit_index, y + row)) {
						V[0xF] = 1;
						_display->set_pixel(x + bit_index, y + row, false);
					}
					else {
						_display->set_pixel(x + bit_index, y + row, true);
					}
				}
			}
		}

		_display->to_update = true;
		break;
	}
	case 0xE:
		switch (NN) {
		case 0x9E:
			if (_input->is_key_active(V[X])) {
				PC += 2;
			}
			break;
		case 0xA1:
			if (!_input->is_key_active(V[X])) {
				PC += 2;
			}
		}
		break;
	case 0xF:
		switch (NN) {
		case 0x7:
			V[X] = delay_timer;
			break;
		case 0xA: {
			bool is_key_pressed = false;
			for (bool key : _input->active_keys) {
				if (key) {
					is_key_pressed = true;
					break;
				}
			}
			if (!is_key_pressed) {
				PC -= 2;
			}
			break;
		}
		case 0x15:
			delay_timer = V[X];
			break;
		case 0x18:
			sound_timer = V[X];
			break;
		case 0x1E: {
			unsigned short current_I = I;
			I += V[X];
			if (current_I <= 0xFFF && I >= 0x1000 && config::vf_if_i_overflow) {
				V[0xF] = 1;
			}
			break;
		}
		case 0x29:
			I = _memory->get(0x50 + ((V[X] & 0xF) * 5));
			break;
		case 0x33: {
			unsigned short temp_VX = V[X];
			std::vector<int> digits{};
			while (temp_VX != 0) {
				digits.push_back(temp_VX % 10);
				temp_VX /= 10;
			}
			std::reverse(digits.begin(), digits.end());
			int i = 0;
			for (int digit : digits) {
				_memory->set(I + i, digit);
				i++;
			}
			break;
		}
		case 0x55: {
			for (int i = 0; i <= X; i++) {
				_memory->set(I + i, V[i]);
				if (config::FX5565_inc_i) {
					I = I + X + 1;
				}
			}
			break;
		}
		case 0x65:
			for (int i = 0; i <= X; i++) {
				V[i] = _memory->get(I + i);
				if (config::FX5565_inc_i) {
					I = I + X + 1;
				}
			}
			break;
		}
		break;
	default:
		std::cout << "Unimplemented opcode -> " << instruction << "\n";
	}

	return true;
}

void CPU::cycle(int times) {
	for (int i = 0; i < times; i++) {
		unsigned short instruction = fetch();
		decode_execute(instruction);
	}
}