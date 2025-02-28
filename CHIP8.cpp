#include "CHIP8.h"

CHIP8::CHIP8()
{
	this->clear_memory();
	this->clear_display();
	this->pc = 0x200;
}

CHIP8::~CHIP8()
{

}

void CHIP8::clear_display()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			this->display[i][j] = '0';
		}
	}
}

void CHIP8::clear_memory()
{
	memset(this->memory, 0, 4096);
	memset(this->V, 0, 16);
	memset(this->stack, 0, 16);
	memset(this->key, 0, 16);
}


bool CHIP8::load_file(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		std::streamsize size = file.tellg();
		this->file_stop = size + 0x200;
		file.seekg(0, std::ios::beg);
		file.read(reinterpret_cast<char*>(&memory[0x200]), size);
		
		return true;
	}
	return false;
}

void CHIP8::execute_opcode(uint16_t code)
{
	printf("> 0x%04x\n", code);
	this->pc = this->pc + 2;
}

unsigned char CHIP8::get_display(int x, int y)
{
	if (x > HEIGHT || x < 0)
		return '0';
	if (y > WIDTH || y < 0)
		return '0';

	return this->display[x][y];
}

bool CHIP8::update()
{
	if (this->pc >= this->file_stop)
		return false;

	uint16_t opcode = this->memory[this->pc] << 8 | this->memory[this->pc + 1];
	this->execute_opcode(opcode);

	if (this->delay_timer > 0)
		this->delay_timer--;

	if (this->sound_timer > 0)
		this->sound_timer--;
	
	return true;
}

void CHIP8::reset_data()
{
	this->clear_memory();
	this->clear_display();
	this->pc = 0x200;
}

void CHIP8::reset_program()
{
	this->clear_display();
	memset(this->V, 0, 16);
	memset(this->stack, 0, 16);
	memset(this->key, 0, 16);
	this->pc = 0x200;
}
