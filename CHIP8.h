#ifndef CHIP_8
#define CHIP_8

#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>
#include <random>

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
	uint8_t display[HEIGHT * WIDTH];

	uint8_t delay_timer;
	uint8_t sound_timer;

	uint8_t key[16];

	uint16_t I;

	uint16_t file_stop;

	bool running;
	bool jmp_flag;

	void execute_opcode(uint16_t code);

public:

	CHIP8();

	~CHIP8();

	void clear_memory();

	bool load_file(const std::string &file_path);

	unsigned char get_display(int x, int y);

	bool update();

	void reset_data();

	void reset_program();

	void setKeyStatus(int i, int status);
};

#endif

