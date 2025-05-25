/* vim: set ft=c noexpandtab fileencoding=utf-8 nomodified wrap textwidth=0 foldmethod=marker foldmarker={{{,}}} foldcolumn=4 ruler showcmd lcs=tab\:|- list: tabstop=8 linebreak showbreak=»\   */
// ,,g = gcc, exactly one space after "set"
//
#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include "colors.h"
#include "flags.h"
#define CM const __memx 
typedef struct b3_t {	 // {{{ // === 24-bit pointer type ===
	uint8_t lo;
	uint8_t hi;
	uint8_t bank;
} b3_t; // }}}
typedef struct b4_t {	 // {{{ // === 32-bit pointer type ===
	uint8_t lo;
	uint8_t hi;
	uint8_t bank;
	uint8_t zero;
} b4_t; // }}}
typedef union  ptr24_u {	// {{{ b3 x uint32_t x const __memx void* union
	const __memx union ptr24_u* p24;
	b3_t b3;
	b4_t b4;
	uint32_t u32;
} ptr24_u; // }}}
static inline ptr24_u V(ptr24_u p){p.b4.zero=0;return p;};	// Verify, Value, sanitise
static inline ptr24_u P(ptr24_u p){	// {{{ Pointer (dereference a return 3Bytes from there)
	p.b4.zero=0;
	ptr24_u r = {.b3=*(CM b3_t *)p.p24};
	r.b4.zero=0;
	return r;
}	// }}}
#define F(x) PSTR(x)
static inline ptr24_u P24u(uint32_t u) { ptr24_u r; r.u32=u; return r;}
static inline ptr24_u P24p(CM void * p) { ptr24_u r; r.p24=(CM ptr24_u *)p;r.b4.zero=0; return r;}
#define DEBUG_DUMP(P,LBL) debug_dump(P24p(P),F(LBL));
#define DEBUG_DUMPu(U,LBL) debug_dump((ptr24_u ){.u32=U},F(LBL));
extern char read_char();
extern void write_char(char c);
extern void write_charA(char c);
extern void write_str(const __memx char *s);
extern void write_eoln();
extern char wait_for_char();
void error(const __memx char *c); 
void dump24(uint32_t w, const __memx char *label);
void write_hex8(uint8_t b);
void write_hex16(uint16_t b);
void write_hex24(uint32_t b);
bool is_ram_address(uint32_t addr);
bool is_flash_address(uint32_t addr);
void debug_dump(const ptr24_u address, const __memx char* label);

typedef struct head1_t {	// {{{
	__memx const struct head1_t *next;		// 3B: pointer to next header "somewhere"
	uint8_t flags;		// 1B: 
	uint8_t len;		// 1B: up to 31 (=5bits)
	const char name[];	// len B:name of WORD
} head1_t;	// }}}
typedef __memx const head1_t *xpHead1;	// 3B pointer to head1 "somewhere"
/*
 * typedef struct head2 {	// {{{
 * 	CodePoint_t codepoint;	// 2B: pointer to function to interpret data
 * 	Data_t data[];	// 3B: pointer to 2B pointer "somewhere" to function to interpret data - pointer to head2 "somewhere"
 * 	} head2;	// }}}
 */

#endif
