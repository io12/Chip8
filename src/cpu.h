#ifndef _CPU_
#define _CPU_

extern uint16_t op;
extern uint8_t m[4096];
extern uint8_t V[16];
extern uint16_t I;
extern uint16_t pc;
extern uint8_t g[2048];
extern uint8_t dt;
extern uint8_t st;
extern uint16_t s[16];
extern uint16_t sp;
extern uint8_t k[16];
extern bool df;

void (*op_ex_table[16])();
void (*rset_table[16])();
void (*arit_table[16])();
void (*skip_table[16])();
void (*load_table[256])();

void op_ex();
void _rset();
void _arit();
void _skip();
void _load();
void _null();
void _00E0();
void _00EE();
void _1nnn();
void _2nnn();
void _3xkk();
void _4xkk();
void _5xy0();
void _6xkk();
void _7xkk();
void _8xy0();
void _8xy1();
void _8xy2();
void _8xy3();
void _8xy4();
void _8xy5();
void _8xy6();
void _8xy7();
void _8xyE();
void _9xy0();
void _Annn();
void _Bnnn();
void _Cxkk();
void _Dxyn();
void _Ex9E();
void _ExA1();
void _Fx07();
void _Fx0A();
void _Fx15();
void _Fx18();
void _Fx1E();
void _Fx29();
void _Fx33();
void _Fx55();
void _Fx65();

#endif
