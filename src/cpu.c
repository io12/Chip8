#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#include "cpu.h"

uint16_t op = 0;
uint8_t m[4096];
uint8_t V[16];
uint16_t I = 0;
uint16_t pc = 0x200;
uint8_t g[2048];
uint8_t dt = 0;
uint8_t st = 0;
uint16_t s[16];
uint16_t sp = 0;
uint8_t k[16];
bool df = 1;

void op_ex() {
	op_ex_table[(op & 0xF000) >> 12]();
}
void (*op_ex_table[])() = {
	_rset, _1nnn, _2nnn, _3xkk, _4xkk, _5xy0, _6xkk, _7xkk,
	_arit, _9xy0, _Annn, _Bnnn, _Cxkk, _Dxyn, _skip, _load
};
void _rset() {
	rset_table[op & 0x000F]();
}
void (*rset_table[])() = {
	_00E0, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _00EE, _null
};
void _arit() {
	arit_table[op & 0x000F]();
}
void (*arit_table[])() = {
	_8xy0, _8xy1, _8xy2, _8xy3, _8xy4, _8xy5, _8xy6, _8xy7,
	_null, _null, _null, _null, _null, _null, _8xyE, _null
};
void _skip() {
	skip_table[op & 0x000F]();
}
void (*skip_table[])() = {
	_null, _ExA1, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _Ex9E, _null
};
void _load() {
	load_table[op & 0x00FF]();
}
void (*load_table[])() = {
	_null, _null, _null, _null, _null, _null, _null, _Fx07,
	_null, _null, _Fx0A, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _Fx15, _null, _null,
	_Fx18, _null, _null, _null, _null, _null, _Fx1E, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _Fx29, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _Fx33, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _Fx55, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _Fx65, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null,
	_null, _null, _null, _null, _null, _null, _null, _null
};

void _null() {
	endwin();
	fprintf(stderr, "Unknown opcode: 0x%04X\n", op);
	exit(1);
}

void _00E0() {
	for (int i = 0; i < 2048; ++i)
		g[i] = 0;
	df = 1;
	pc += 2;
}
void _00EE() {
	--sp;
	pc = s[sp];
	pc += 2;
}
void _1nnn() {
	pc = op & 0x0FFF;
}
void _2nnn() {
	s[sp] = pc;
	++sp;
	pc = op & 0x0FFF;
}
void _3xkk() {
	if (V[(op & 0x0F00) >> 8] == (op & 0x00FF))
		pc += 4;
	else
		pc += 2;
}
void _4xkk() {
	if (V[(op & 0x0F00) >> 8] != (op & 0x00FF))
		pc += 4;
	else
		pc += 2;
}
void _5xy0() {
	if (V[(op & 0x0F00) >> 8] == V[(op & 0x00F0) >> 4])
		pc += 4;
	else
		pc += 2;
}
void _6xkk() {
	V[(op & 0x0F00) >> 8] = op & 0x00FF;
	pc += 2;
}
void _7xkk() {
	V[(op & 0x0F00) >> 8] += op & 0x00FF;
	pc += 2;
}
void _8xy0() {
	V[(op & 0x0F00) >> 8] = V[(op & 0x00F0) >> 4];
	pc += 2;
}
void _8xy1() {
	V[(op & 0x0F00) >> 8] |= V[(op & 0x00F0) >> 4];
	pc += 2;
}
void _8xy2() {
	V[(op & 0x0F00) >> 8] &= V[(op & 0x00F0) >> 4];
	pc += 2;
}
void _8xy3() {
	V[(op & 0x0F00) >> 8] ^= V[(op & 0x00F0) >> 4];
	pc += 2;
}
void _8xy4() {
	if (V[(op & 0x00F0) >> 4] > 0xFF - V[(op & 0x0F00) >> 8])
		V[0xF] = 1; // carry
	else
		V[0xF] = 0;
	V[(op & 0x0F00) >> 8] += V[(op & 0x00F0) >> 4];
	pc += 2;
}
void _8xy5() {
	if (V[(op & 0x00F0) >> 4] > V[(op & 0x0F00) >> 8])
		V[0xF] = 0; // borrow
	else
		V[0xF] = 1;
	V[(op & 0x0F00) >> 8] -= V[(op & 0x00F0) >> 4];
	pc += 2;
}
void _8xy6() {
	V[0xF] = V[(op & 0x0F00) >> 8] & 0x1;
	V[(op & 0x0F00) >> 8] >>= 1;
	pc += 2;
}
void _8xy7() {
	if (V[(op & 0x0F00) >> 8] > V[(op & 0x00F0) >> 4])
		V[0xF] = 0; // borrow
	else
		V[0xF] = 1;
	V[(op & 0x0F00) >> 8] = V[(op & 0x00F0) >> 4] - V[(op & 0x0F00) >> 8];
	pc += 2;
}
void _8xyE() {
	V[0xF] = V[(op & 0x0F00) >> 8] >> 7;
	V[(op & 0x0F00) >> 8] <<= 1;
	pc += 2;
}
void _9xy0() {
	if (V[(op & 0x0F00) >> 8] != V[(op & 0x00F0) >> 4])
		pc += 4;
	else
		pc += 2;
}
void _Annn() {
	I = op & 0x0FFF;
	pc += 2;
}
void _Bnnn() {
	pc = (op & 0x0FFF) + V[0];
}
void _Cxkk() {
	V[(op & 0x0F00) >> 8] = (rand() % 0xFF) & (op & 0x00FF);
	pc += 2;
}
void _Dxyn() {
	uint16_t x = V[(op & 0x0F00) >> 8];
	uint16_t y = V[(op & 0x00F0) >> 4];
	V[0xF] = 0;
	for (int i = 0; i < (op & 0x000F); ++i) {
		uint16_t r = m[I + i];
		for(int j = 0; j < 8; ++j) {
			if ((r & (0x80 >> j)) != 0) {
				if (g[x + j + (y + i) * 64] == 1)
					V[0xF] = 1;                                    
				g[x + j + (y + i) * 64] ^= 1;
			}
		}
	}
	df = 1;
	pc += 2;
}
void _Ex9E() {
	if (k[V[(op & 0x0F00) >> 8]] != 0)
		pc += 4;
	else
		pc += 2;
}
void _ExA1() {
	if (k[V[(op & 0x0F00) >> 8]] == 0)
		pc += 4;
	else
		pc += 2;
}
void _Fx07() {
	V[(op & 0x0F00) >> 8] = dt;
	pc += 2;
}
void _Fx0A() {
	for (int i = 0; i < 16; ++i) {
		if (k[i] == 1) {
			V[(op & 0x0F00) >> 8] = i;
			pc += 2;
			break;
		}
	}
}
void _Fx15() {
	dt = V[(op & 0x0F00) >> 8];
	pc += 2;
}
void _Fx18() {
	st = V[(op & 0x0F00) >> 8];
	pc += 2;
}
void _Fx1E() {
	if (I + V[(op & 0x0F00) >> 8] > 0xFFF)
		V[0xF] = 1;
	else
		V[0xF] = 0;
	I += V[(op & 0x0F00) >> 8];
	pc += 2;
}
void _Fx29() {
	I = V[(op & 0x0F00) >> 8] * 0x5;
	pc += 2;
}
void _Fx33() {
	m[I] = V[(op & 0x0F00) >> 8] / 100;
	m[I + 1] = V[(op & 0x0F00) >> 8] / 10 % 10;
	m[I + 2] = V[(op & 0x0F00) >> 8] % 100 % 10;
	pc += 2;
}
void _Fx55() {
	m[I] = V[(op & 0x0F00) >> 8] / 100;
	m[I + 1] = V[(op & 0x0F00) >> 8] / 10 % 10;
	m[I + 2] = V[(op & 0x0F00) >> 8] % 100 % 10;
	pc += 2;
}
void _Fx65() {
	for (int i = 0; i <= ((op & 0x0F00) >> 8); ++i)
		V[i] = m[I + 1];
	I += ((op & 0x0F00) >> 8) + 1;
	pc += 2;
}
