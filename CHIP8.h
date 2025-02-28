#ifndef CHIP_8
#define CHIP_8

#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>

#define HEIGHT 32
#define WIDTH 64

class CHIP8
{
private:
	uint8_t memory[4096];
	uint16_t pc;

	uint8_t V[16];
	uint16_t stack[16];

	uint8_t *sp;
	unsigned char display[HEIGHT][WIDTH];

	uint8_t delay_timer;
	uint8_t sound_timer;

	uint8_t key[16];

	uint16_t file_stop;

public:

	CHIP8();

	~CHIP8();

	void clear_display();

	void clear_memory();

	bool load_file(const std::string &file_path);

	void execute_opcode(uint16_t code);

	unsigned char get_display(int x, int y);

	bool update();

	void reset_data();

	void reset_program();
};

#endif

