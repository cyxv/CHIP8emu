#pragma once

namespace config {
	const int cpu_cycles_per_frame = 11;
	const int pixel_size = 16;

	const bool shift_vx_equals_vy = false;
	const bool add_vx_to_offset_jmp = false;
	const bool vf_if_i_overflow = false;
	const bool FX5565_inc_i = false;
}