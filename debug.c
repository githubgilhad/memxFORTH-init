/* vim: set ft=c noexpandtab fileencoding=utf-8 nomodified wrap textwidth=0 foldmethod=marker foldmarker={{{,}}} foldcolumn=4 ruler showcmd lcs=tab\:|- list: tabstop=8 linebreak showbreak=»\   */
// ,,g = gcc, exactly one space after "set"
#include "debug.h"
//
extern void write_char(char c);
extern void write_charA(char c);
//
void write_str(const __memx char *s) { 	// {{{
	while (*s) write_char(*s++);
}	// }}}
void write_eoln() { 	// {{{
	write_char('\r');
	write_char('\n');
}	// }}}

char wait_for_char() {	// {{{
	char c=0;
	while (c ==0) {c=read_char();};
	write_char(c);
	return c;
}	// }}}

void error(const __memx char *c) { 	// {{{
	write_str(F(BG_RED CLR_BLUE STYLE_BOLD "«" )); //0xC2 0xAB);//'«'
	while (*c){write_char(*c++);};
	write_str(F("»" CLR_RESET "\r\n"));// 0xC2 0xBB);//'»'
}	// }}}

const __memx char hex[] = "0123456789ABCDEF";
void write_hex8(uint8_t b) {
	write_char(hex[(b >> 4) & 0xF]);
	write_char(hex[b & 0xF]);
}

void write_hex16(uint16_t w) {
	write_hex8(w >> 8);
	write_hex8(w & 0xFF);
}

void write_hex24(uint32_t w) {
	write_hex8((w >> 16) & 0xFF);
	write_hex8((w >> 8) & 0xFF);
	write_hex8(w & 0xFF);
}
void write_hex32(uint32_t w) {
	write_hex8((w >> 24) & 0xFF);
	write_hex8((w >> 16) & 0xFF);
	write_hex8((w >> 8) & 0xFF);
	write_hex8(w & 0xFF);
}


#define RAM_START     ((uint32_t)0x800100)
#define RAM_END       ((uint32_t)0x810000)
#define FLASH_START   ((uint32_t)0x000000)
#define FLASH_END     ((uint32_t)0x040000)

bool is_ram_address(uint32_t addr) {	 // {{{
	return addr >= RAM_START && addr < RAM_END;
}	// }}}

bool is_flash_address(uint32_t addr) {	 // {{{
	return ((addr >= FLASH_START) && (addr < FLASH_END));
}	// }}}

static inline uint8_t read_memx(uint32_t addr) {	 // {{{
	ptr24_u x; x.u32=addr;
	return *(volatile const __memx uint8_t*)x.p24;
}	// }}}
void debug_dump(const  ptr24_u address, const __memx char* label) {	 // {{{
	uint32_t addr = address.u32;
	uint32_t base = addr & ~0x0F;
	uint32_t start = (base >= 16) ? base - 16 : base;

	write_str(F("{" CLR_YELLOW));
	write_str(label);
	write_str(F(CLR_RESET "\t [" CLR_GREEN));
	write_hex32(addr);
	write_str(F(CLR_RESET "] -> "));

	for (uint8_t i = 0; i < 3*16; ++i) {
		if( i==16 || i==32 ) write_str(F(" | "));
		uint32_t curr = start + i;
		bool in_range = is_ram_address(curr) || is_flash_address(curr);
		if (curr == addr) write_str(F(BG_GREEN));
		if (in_range) { write_hex8(read_memx(curr));
		} else { write_str(F("  ")); };
		if (curr == addr) write_str(F(CLR_RESET));
		write_char(' ');
	}

	write_str(F(" || '"));

	// ASCII
	char c;
	for (uint8_t i = 0; i < 3*16; ++i) {
		if( i==16 || i==32 ) write_str(F("' | '"));
		uint32_t curr = start + i;
		bool in_range = is_ram_address(curr) || is_flash_address(curr);

		if (in_range) {
			c = read_memx(curr);
		} else {
			c = ' ';
		}

		if (curr == addr) {
			write_str(F(BG_GREEN));
			write_charA(c);
			write_str(F(CLR_RESET));
		} else {
			write_charA(c);
		}
	}
	write_str(F("' }\r\n"));
}	// }}}
