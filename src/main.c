#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <curses.h>
#include <unistd.h>

#include "cpu.h"

int main(int argc, char* argv[]) {
	if (argc == 0) {
		fprintf(stderr, "usage: %s [rom]\n", argv[0]);
		return 1;
	}
	// fontset
	uint8_t fs[80] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, //0
		0x20, 0x60, 0x20, 0x20, 0x70, //1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
		0x90, 0x90, 0xF0, 0x10, 0x10, //4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
		0xF0, 0x10, 0x20, 0x40, 0x40, //7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
		0xF0, 0x90, 0xF0, 0x90, 0x90, //A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
		0xF0, 0x80, 0x80, 0x80, 0xF0, //C
		0xE0, 0x90, 0x90, 0x90, 0xE0, //D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
		0xF0, 0x80, 0xF0, 0x80, 0x80  //F	
	};
	// ncurses init
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	nodelay(stdscr, 1);
	// clear display
	for(int i = 0; i < 2048; ++i)
		g[i] = 0;
	// clear other things
	for(int i = 0; i < 16; ++i) {
		k[i] = 0;
		V[i] = 0;
		s[i] = 0;
	}
	// clear memory
	for(int i = 0; i < 4096; ++i)
		m[i] = 0;
	// load fontset
	for(int i = 0; i < 80; i++)
		m[i] = fs[i];
	srand(time(NULL));
	// load game
	FILE* fp = fopen(argv[1], "rb");
	if (!fp) {
		endwin();
		fprintf(stderr, "File \"%s\" not found\n", argv[1]);
		return 1;
	}
	fseek(fp, 0, SEEK_END);
	uint32_t fsize = ftell(fp);
	if (fsize > (3584)) {
		endwin();
		fprintf(stderr, "Invalid rom. %d bytes > 3584\n", fsize);
		return 1;
	}
	rewind(fp);
	char* b = (char*) malloc(fsize);
	if (!b) {
		endwin();
		fprintf(stderr, "Failed to allocate %d bytes\n", fsize);
		return 1;
	}
	if (fread(b, 1, fsize, fp) != fsize) {
		endwin();
		fprintf(stderr, "Error reading file\n");
		return 1;
	}
	for(int i = 0; i < fsize; ++i)
		m[i + 512] = b[i];
	fclose(fp);
	free(b);
	for(;;) {
		switch (getch()) {
			case '1':
				k[0x1] = 1;
				break;
			case '2':
				k[0x2] = 1;
				break;
			case '3':
				k[0x3] = 1;
				break;
			case '4':
				k[0xC] = 1;
				break;
			case 'q':
				k[0x4] = 1;
				break;
			case 'w':
				k[0x5] = 1;
				break;
			case 'f':
				k[0x6] = 1;
				break;
			case 'p':
				k[0xD] = 1;
				break;
			case 'a':
				k[0x7] = 1;
				break;
			case 'r':
				k[0x8] = 1;
				break;
			case 's':
				k[0x9] = 1;
				break;
			case 't':
				k[0xE] = 1;
				break;
			case 'z':
				k[0xA] = 1;
				break;
			case 'x':
				k[0x0] = 1;
				break;
			case 'c':
				k[0xB] = 1;
				break;
			case 'v':
				k[0xF] = 1;
				break;
		}
		op = m[pc] << 8 | m[pc + 1];
		//fprintf(stderr, "%X\n", op);
		op_ex();
		for (int i = 0; i < 16; ++i)
			k[i] = 0;
		// timers
		if (dt > 0)
			--dt;
		if (st == 1)
			beep();
		if (st > 0)
			--st;
		// print frame
		erase();
		move(0, 0);
		if (COLS < 64 || LINES < 32) {
			printw("Window too small");
			continue;
		}
		for (int i = 0; i < 32; ++i) {
			for (int j = 0; j < 64; ++j) {
				if (g[i * 64 + j] == 1)
					attron(A_STANDOUT);
				if (COLS >= 128)
					printw("  ");
				else
					printw(" ");
				attroff(A_STANDOUT);
			}
			printw("\n");
		}
		if (df) {
			refresh();
			df = 0;
		}
		if (argc == 3)
			sleep(1);
		else
			usleep(16667);
	}
	return 0;
}
