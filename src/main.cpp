#include "display.h"
#include "memory.h"
#include "CPU.h"
#include "config.h"
#include "input.h"

int main(int argc, char** argv) {
	Display display{};
	Memory memory{};
	Input input{};
	CPU cpu{ &memory, &display, &input };

	if (argc != 2) {
		std::cout << "Please supply a ROM.\n";
		return 1;
	}
	memory.load_rom(argv[1]);

	display.begin();
	while (!WindowShouldClose()) {
		input.update();
		cpu.cycle(config::cpu_cycles_per_frame);
		display.draw();

		if (cpu.delay_timer > 0)
			cpu.delay_timer--;
		if (cpu.sound_timer > 0) {
			cpu.sound_timer--;
			// TODO: implement beep
		}
		std::cout << "FPS: " << 1000 / (GetFrameTime() * 1000) << "\n";
	}
	display.end();
}