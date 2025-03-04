#include "CHIP8.h"

//#define DEBUG

CHIP8::CHIP8()
{
	this->clear_memory();
	this->reset_program();

	/*
	this->pc = 0x200;
	this->sp = (uint8_t *) this->stack;
	this->running = true;
	this->jmp_flag = false;
	*/

	std::srand(std::time(nullptr));

}

CHIP8::~CHIP8()
{

}

void CHIP8::clear_memory()
{
	memset(this->memory, 0, 4096);
	/*
	memset(this->V, 0, 16);
	memset(this->stack, 0, 16);
	memset(this->key, 0, 16);
	memset(this->display, 0, WIDTH * HEIGHT);
	*/

	uint8_t sprites[80] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
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
		0xF0, 0x80, 0x80, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80 // F
	};
	for (int i = 0; i <= 80; i++)
	{
		this->memory[i] = sprites[i];
	}
}


bool CHIP8::load_file(const std::string& filename)
{
	this->clear_memory();
	this->reset_program();

	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		std::streamsize size = file.tellg();
		this->file_stop = size + 0x200;
		file.seekg(0, std::ios::beg);
		file.read(reinterpret_cast<char*>(&memory[0x200]), size);
		
		this->pc = 0x200;

		printf("[+] Loaded CHIP-8 rom: %s (size %d bytes)\n", filename.c_str(), size);

		return true;
	}

	this->running = false;
	this->pc = 0x200;
	this->file_stop = this->pc;

	printf("[-] Failed to load CHIP-8 rom\n");

	return false;
}

void CHIP8::execute_opcode(uint16_t code)
{
	printf("[%04x] - (0x%04x) - ", this->pc, code);

	switch (code & 0xF000)
	{
		case 0x0000: 
			switch (code)
			{
				case 0x00E0:
					printf("CLS");
					/*
						00E0 - CLS
						Clear the display.
					*/
					
					memset(this->display, 0, WIDTH * HEIGHT);
					break;
				case 0x00EE:
					printf("RET");
					/*
						00EE - RET
						Return from a subroutine.
						The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.
					*/
					this->sp -= 1;
					this->pc = *this->sp;
					*this->sp = 0x0000;
					//this->jmp_flag = true;
					;
					break;
			};
			break;
		case 0x1000:
			printf("JP 0x%04x", code & 0x0FFF);
			/*
				1nnn - JP addr
				Jump to location nnn.
				The interpreter sets the program counter to nnn.
			*/;
			this->pc = code & 0x0FFF;
			this->jmp_flag = true;
			break;
		case 0x2000:
			printf("CALL 0x%04x", code & 0x0FFF);
			/*
				2nnn - CALL addr
				Call subroutine at nnn.
				The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
			*/
			*this->sp = this->pc;
			this->sp += 1;
			this->pc = code % 0x0FFF - 2;
			this->jmp_flag = true;
			break;
		case 0x3000:
			printf("SE V%d, 0x%02x", (code & 0x0F00) >> 8, (code & 0x00FF));
			/*
				3xkk - SE Vx, byte
				Skip next instruction if Vx = kk.
				The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
			*/
			if (this->V[(code & 0x0F00) >> 8] == (code & 0x00FF) )
				this->pc += 2;
			break;
		case 0x4000:
			printf("SNE V%d, 0x%02x", (code & 0x0F00) >> 8, (code & 0x00FF));
			/*
				4xkk - SNE Vx, byte
				Skip next instruction if Vx != kk.
				The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
			*/
			if (this->V[(code & 0x0F00) >> 8] != (code & 0x00FF))
				this->pc += 2;
			break;
		case 0x5000:
			printf("SE V%d, V%d", (code & 0x0F00) >> 8, (code & 0x00F0) >> 4);
			/*
				5xy0 - SE Vx, Vy
				Skip next instruction if Vx = Vy.
				The interpreter compares register Vx to register Vy, and if they are equal, increments the program counter by 2.
			*/
			if (this->V[(code & 0x0F00) >> 8] == this->V[(code & 0x00F0) >> 4])
				this->pc += 2;
			break;
		case 0x6000:
			printf("LD V%d, 0x%02x", (code & 0x0F00) >> 8, code & 0x00FF);
			/*
				6xkk - LD Vx, byte
				Set Vx = kk.
				The interpreter puts the value kk into register Vx.
			*/
			this->V[(code & 0x0F00) >> 8] = (code & 0x00FF);
			;
			break;
		case 0x7000:
			printf("ADD V%d, 0x%02x", (code & 0x0F00) >> 8, (code & 0x00FF) );
			/*
				7xkk - ADD Vx, byte
				Set Vx = Vx + kk.
				Adds the value kk to the value of register Vx, then stores the result in Vx.
			*/
			this->V[(code & 0x0F00) >> 8] += (code & 0x00FF);
			;
			break;

		case 0x8000:
			//	0 - F
			switch (code & 0x000F)
			{
				case 0x0:
					printf("LD V%d, V%d", (code & 0x0F00) >> 8, (code & 0x00F0) >> 4);
					/*
						8xy0 - LD Vx, Vy
						Set Vx = Vy.
						Stores the value of register Vy in register Vx.
					*/
					this->V[(code & 0x0F00) >> 8] = this->V[(code & 0x00F0) >> 4];;
					break;
				case 0x1:
					printf("OR V%d, V%d", (code & 0x0F00) >> 8, (code & 0x00F0) >> 4);
					/*
						8xy1 - OR Vx, Vy
						Set Vx = Vx OR Vy.
					*/
					this->V[(code & 0x0F00) >> 8] = this->V[(code & 0x0F00) >> 8] | this->V[(code & 0x00F0) >> 4];
					break;
				case 0x2:
					printf("AND V%d, V%d", (code & 0x0F00) >> 8, (code & 0x00F0) >> 4);
					/*
						8xy2 - AND Vx, Vy
						Set Vx = Vx AND Vy.
					*/
					this->V[(code & 0x0F00) >> 8] = this->V[(code & 0x0F00) >> 8] & this->V[(code & 0x00F0) >> 4];
					break;
				case 0x3:
					printf("XOR V%d, V%d", (code & 0x0F00) >> 8, (code & 0x00F0) >> 4);
					/*
						8xy3 - XOR Vx, Vy
						Set Vx = Vx XOR Vy.
					*/
					this->V[(code & 0x0F00) >> 8] = this->V[(code & 0x0F00) >> 8] ^ this->V[(code & 0x00F0) >> 4];
					break;
				case 0x4:
					printf("ADD V%d, V%d", (code & 0x0F00) >> 8, (code & 0x00F0) >> 4);
					/*
						8xy4 - ADD Vx, Vy
						Set Vx = Vx + Vy, set VF = carry.
						The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.
					*/

					//uint16_t temp = this->V[(code & 0x0F00) >> 8] + this->V[(code & 0x00F0) >> 4];
					if (this->V[(code & 0x0F00) >> 8] + this->V[(code & 0x00F0) >> 4] > 255)
						this->V[0xF] = 1;
					this->V[(code & 0x0F00) >> 8] = this->V[(code & 0x0F00) >> 8] + this->V[(code & 0x00F0) >> 4] & 0xFFFF;
					;
					break;
				case 0x5:
					printf("SUB V%d, V%d", (code & 0x0F00) >> 8, (code & 0x00F0) >> 4);
					/*
						8xy5 - SUB Vx, Vy
						Set Vx = Vx - Vy, set VF = NOT borrow.
						If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
					*/
					if (this->V[(code & 0x0F00) >> 8] > this->V[(code & 0x00F0) >> 4])
					{
						this->V[0xF] = 1;
					}
					else
					{
						this->V[0xF] = 0;
					}

					this->V[(code & 0x0F00) >> 8] = this->V[(code & 0x0F00) >> 8] - this->V[(code & 0x00F0) >> 4];
					break;
				case 0x6:
					printf("SHR V%d", (code & 0x0F00) >> 8);
					/*
						8xy6 - SHR Vx {, Vy}
						Set Vx = Vx SHR 1.	
					*/
					this->V[0xF] = this->V[(code & 0x0F00) >> 8] & 0b0001;
					this->V[(code & 0x0F00) >> 8] = this->V[(code & 0x0F00) >> 8] >> 1;
					break;
				case 0x7:
					printf("SUBN V%d, V%d", (code & 0x0F00) >> 8, (code & 0x00F0) >> 4);
					/*
						8xy7 - SUBN Vx, Vy
						Set Vx = Vy - Vx, set VF = NOT borrow.
						If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
					*/
					if (this->V[(code & 0x00F0) >> 4] > this->V[(code & 0x0F00) >> 8])
					{
						this->V[0xF] = 1;
					}
					else
					{
						this->V[0xF] = 0;
					}

					this->V[(code & 0x0F00) >> 8] = this->V[(code & 0x00F0) >> 4] - this->V[(code & 0x0F00) >> 8] ;
					break;
				case 0xE:
					printf("SHL V%d", (code & 0x0F00) >> 8);
					/*
						8xyE - SHL Vx {, Vy}
						Set Vx = Vx SHL 1.
						If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2.
					*/
					this->V[0xF] = (this->V[(code & 0x0F00) >> 8] & 0b1000000000000000) >> 15;
					this->V[(code & 0x0F00) >> 8] = this->V[(code & 0x0F00) >> 8] << 1;
					break;
				default:
					printf("unknown code\n");
					break;
			}
			;
			break;
		case 0x9000:
			printf("SNE V%d, V%d", (code & 0x0F00) >> 8, (code & 0x00F0) >> 4);
			/*
				9xy0 - SNE Vx, Vy
				Skip next instruction if Vx != Vy.
				The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
			*/
			if (this->V[(code & 0x0F00) >> 8] != this->V[(code & 0x00F0) >> 4])
				this->pc += 2;
			break;
		case 0xA000:
			printf("LD I, 0x%04x", code & 0x0FFF);
			/*
				Annn - LD I, addr
				Set I = nnn.
				The value of register I is set to nnn.
			*/
			this->I = (code & 0x0FFF);
			break;
		case 0xB000:
			printf("JP V0, 0x%04x", (code & 0x0FFF) );
			/*
				Bnnn - JP V0, addr
				Jump to location nnn + V0.
			*/
			this->pc = (code & 0x0FFF) + this->V[0];
			this->jmp_flag = true;
			break;
		case 0xC000:
			printf("RND V%d, 0x%02x", (code & 0x0F00) >> 8, (code & 0x00FF) );
			/*
				Cxkk - RND Vx, byte
				Set Vx = random byte AND kk.
			*/
			this->V[(code & 0x0F00) >> 8] = (rand() % 255) & (code & 0x00FF);
			break;
		case 0xD000:
			printf("DRW V%d, V%d, %d", (code & 0x0F00) >> 8, (code & 0x00F0) >> 4, code & 0x000F);
			/*
				Dxyn - DRW Vx, Vy, nibble
				Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
			*/
			{
				uint8_t xPos = this->V[(code & 0x0F00) >> 8] % WIDTH;
				uint8_t yPos = this->V[(code & 0x00F0) >> 4] % HEIGHT;
				uint8_t N = code & 0x000F;

				this->V[0xF] = 0;

				for (uint8_t row = 0; row < N; row++)
				{
					uint8_t spriteByte = this->memory[this->I + row];
					for (uint8_t col = 0; col < 8; col++)
					{
						if ((spriteByte & (0x80 >> col)) != 0)
						{
							uint16_t pixelIndex = (xPos + col) + ((yPos + row) * WIDTH);

							if (this->display[pixelIndex] == 1) {
								V[0xF] = 1; 
							}

							this->display[pixelIndex] ^= 1; 

						}
					}
				}
			}
			;
			break;
		case 0xE000:
			switch (code & 0xF0FF)
			{
				case 0xE09E:
					printf("SKP V%d", (code & 0x0F00) >> 8);
					/*
						Ex9E - SKP Vx
						Skip next instruction if key with the value of Vx is pressed.
						Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
					*/
					if (this->key[this->V[ (code & 0x0F00) >> 8] ] == 1)
						this->pc += 2;
					break;
				case 0xE0A1:
					printf("SKNP V%d", (code & 0x0F00) >> 8);
					/*
						ExA1 - SKNP Vx
						Skip next instruction if key with the value of Vx is not pressed.
						Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
					*/
					if (this->key[this->V[(code & 0x0F00) >> 8] ] == 0)
						this->pc += 2;
					break;
				default:
					printf("unknown code\n");
					break;
			}
			;
			break;
		case 0xF000:
			switch (code & 0xF0FF)
			{
				case 0xF007:
					printf("LD V%d, DT", (code & 0x0F00) >> 8);
					/*
						Fx07 - LD Vx, DT
						Set Vx = delay timer value.
					*/
					this->V[(code & 0x0F00) >> 8] = this->delay_timer;
					break;
				case 0xF00A:
					{
						printf("LD V%d, K", (code & 0x0F00) >> 8);
						/*
							Fx0A - LD Vx, K
							Wait for a key press, store the value of the key in Vx.
						*/
						bool flag = false;

						for (uint8_t i = 0; i < 16; i++)
						{
							if (key[i])
							{
								this->V[(code & 0x0F00) >> 8] = i;
								this->running = true;
								flag = true;
								break;
							}
						}

						if (flag)
							break;
						else
							this->running = false;
						break;
					}
				case 0xF015:
					printf("LD DT, V%d", (code & 0x0F00) >> 8);
					/*
						Fx15 - LD DT, Vx
						Set delay timer = Vx.
					*/
					this->delay_timer = this->V[(code & 0x0F00) >> 8];
					break;
				case 0xF018:
					printf("LD ST, V%d", (code & 0x0F00) >> 8);
					/*
						Fx18 - LD ST, Vx
						Set sound timer = Vx.
					*/
					this->sound_timer = this->V[(code & 0x0F00) >> 8];
					break;
				case 0xF01E:
					printf("ADD I, V%d", (code & 0x0F00) >> 8);
					/*
						Fx1E - ADD I, Vx
						Set I = I + Vx.
						The values of I and Vx are added, and the results are stored in I.
					*/
					this->I = this->I + this->V[(code & 0x0F00) >> 8];
					break;
				case 0xF029:
					printf("LD F, V%d", (code & 0x0F00) >> 8);
					/*
						Fx29 - LD F, Vx
						Set I = location of sprite for digit Vx.
					*/
					this->I = this->V[( code & 0x0F00 ) >> 8] * 5;
					;
					break;
				case 0xF033:
					printf("LD B, V%d", (code & 0x0F00) >> 8);
					/*
						Fx33 - LD B, Vx
						Store BCD representation of Vx in memory locations I, I+1, and I+2.
					*/
					{
						uint8_t value = this->V[(code & 0x0F00) >> 8];

						this->memory[this->I] = int(value / 100) % 10;		//	huns
						this->memory[this->I + 1] = int(value / 10) % 10;	//	tens
						this->memory[this->I + 2] = value % 10;				//	ones
					}
					;
					break;
				case 0xF055:
					printf("LD [I], V%d", (code & 0x0F00) >> 8);
					/*
						Fx55 - LD [I], Vx
						Store registers V0 through Vx in memory starting at location I.
					*/
					{
						uint8_t X = (code & 0x0F00) >> 8;
						for (uint8_t i = 0; i <= X; i++) {
							this->memory[this->I + i] = this->V[i];
						}
					}
					;
					break;
				case 0xF065:
					printf("LD V%d, [I]", (code & 0x0F00) >> 8);
					/*
						Fx65 - LD Vx, [I]
						Read registers V0 through Vx from memory starting at location I.
					*/
					{
						uint8_t X = (code & 0x0F00) >> 8;
						for (uint8_t i = 0; i <= X; i++) 
							this->V[i] = this->memory[this->I + i];
					}
					;
					break;
				default:
					printf("unknown code\n");
					break;
					
			}
			break;
		default: 
			printf("unknown code\n");
			break;
		
	}

	printf("\n");

	#ifdef DEBUG
		printf("[DEBUG]\n");
		printf("V0=#%02x\tV1=#%02x\tV2=#%02x\tV3=#%02x\nV4=#%02x\tV5=#%02x\tV6=#%02x\tV7=#%02x\nV8=#%02x\tV9=#%02x\tV10=#%02x\tV11=#%02x\nV12=#%02x\tV13=#%02x\tV14=#%02x\tV15=#%02x\n", 
			this->V[0], this->V[1], this->V[2], this->V[3], this->V[4], this->V[5], this->V[6], this->V[7], this->V[8], this->V[9], this->V[10], this->V[11], this->V[12], this->V[13],
			this->V[14], this->V[15]);
		printf("I=%02x\tPC=%02x", this->I, this->pc);
		printf("\n\n");
	#endif 

	if (this->running)
	{
		if (this->jmp_flag)
			this->jmp_flag = false;
		else
			this->pc = this->pc + 2;
	}

}

uint8_t CHIP8::get_display(int x, int y)
{

	return this->display[x + y * WIDTH];
}

bool CHIP8::update()
{
	if (this->pc >= this->file_stop)
		return false;


	uint16_t opcode = this->memory[this->pc] << 8 | this->memory[this->pc + 1];
	this->execute_opcode(opcode);

	if (this->delay_timer > 0)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1000));
		this->delay_timer--;
	}


	if (this->sound_timer > 0)
		this->sound_timer--;
	
	std::this_thread::sleep_for(std::chrono::nanoseconds(150));
	
	return true;
}

void CHIP8::reset_data()
{
	this->clear_memory();
	this->pc = 0x200;
}

void CHIP8::reset_program()
{
	memset(this->display, 0, WIDTH * HEIGHT);
	memset(this->V, 0, 16);
	memset(this->stack, 0, 16);
	memset(this->key, 0, 16);
	
	this->pc = 0x200;
	this->sp = (uint16_t *)this->stack;
	this->running = true;
	this->jmp_flag = false;

}

void CHIP8::setKeyStatus(int i, int status)
{
	this->key[i] = status;
}

bool CHIP8::is_sound()
{
	if (this->sound_timer > 0)
		return true;
	return false;
}
