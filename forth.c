/* vim: set ft=c noexpandtab fileencoding=utf-8 nomodified wrap textwidth=0 foldmethod=marker foldmarker={{{,}}} foldcolumn=4 ruler showcmd lcs=tab\:|- list: tabstop=8 linebreak showbreak=»\   */
// ,,g = gcc, exactly one space after "set"

#include <stdint.h>
#include <stddef.h>
#include "debug.h"

void f_next(void);	// FORWARD

extern __memx const char w_test_data;
extern __memx const head1_t top_head;

#define NEXT f_next()
typedef uint32_t CELL_t ;
typedef ptr24_u PTR_t ;

ptr24_u IP;
ptr24_u DT;		// for f_docol to know, which _cw was called
#define STACK_LEN 10
#define RAM_LEN 100	// 8B + name + 3B * (# words in definition)
CELL_t stck[STACK_LEN];
CELL_t *stack=&stck[STACK_LEN];
PTR_t Rstck[STACK_LEN];
PTR_t *Rstack=&Rstck[STACK_LEN];

//xpHead1 LAST;
CM head1_t *LAST;
char RAM[RAM_LEN];
char *HERE=&RAM[0];

// {{{ pop
CELL_t pop() {
	error(F("pop"));
	write_hex24(*stack);
	return *(stack++);
}
void push(CELL_t x) {
	error(F("push")); 
	*(--stack)=x;
	write_hex24(x);
}
CELL_t peek(){ return *(stack);}
// }}}
// {{{ Rpop
PTR_t Rpop() {
	error(F("Rpop"));
	write_hex24((*Rstack).u32);
	return *(Rstack++);
}
void Rpush(PTR_t x) { 
	error(F("Rpush"));
	*(--Rstack)=x;
	write_hex24(x.u32);
}
PTR_t Rpeek(){ return *(Rstack);}
// }}}
void f_dup(){	// {{{
	error(F("f_dup"));
	push(peek());
	NEXT;
}	// }}}
void f_plus(){	// {{{
	error(F("f_plus"));
	push(pop()+pop());
	NEXT;
}	// }}}

void f_next(void) {	// {{{ // === f_next: fetch next codeword from body and execute it ===
	error(F("f_next"));
	debug_dump(IP,F("IP in\t"));
	DEBUG_DUMP(stack,"stack\t");
	DEBUG_DUMP(Rstack,"Rstack\t");
	write_eoln();
	DT=P(IP);
	IP.u32+=3;
	debug_dump(IP,F("IP out\t"));
	debug_dump(P(IP),F("*IP out"));
//	debug_dump(P(P(IP)),F("**IP out"));
	debug_dump(DT,F("DT\t"));
	ptr24_u FN={.u32=P(DT).u32/2};
	debug_dump(FN,F("FN\t"));
	write_str(F("Press a key ..."));
	wait_for_char();
	((void(*)()) FN.p24)();
}	// }}}

void f_docol(void) {	// {{{  // === f_docol: begin execution of a colon word ===
	error(F("f_docol"));
	Rpush(IP);
	IP=DT;
	IP.u32+=3;
	debug_dump(IP,F("IP docol"));
	f_next();
}	// }}}

void f_exit(void) {	// {{{ // === f_exit: return from colon word ===
	error(F("f_exit"));
	IP = Rpop();
	f_next();
}	// }}}
void f_debug(void) {	// {{{ // === f_debug: return from FOTH or what ===
	error(F("f_debug"));
	// f_next();	// No, simply no, return back to caler ...
}	// }}}
void print_words(void) {	// {{{ // === print all wocabulary
	xpHead1 h=LAST;
	while (h) {
//		debug_dump(P24p(h),F("h"));
		if (h->flags & FLG_HIDDEN) write_str(F(CLR_GREY));
		for (uint8_t i = 0; i < h->len; ++i) write_char(h->name[i]);
		if (h->flags & FLG_HIDDEN) write_str(F(CLR_RESET));
		write_char(' ');
		h= h->next;
		};
	write_eoln();
}	// }}}
void f_words(void) {	// {{{ print all words
	error(F("f_words"));
	print_words();
	f_next();
}	// }}}

const __memx char f_words_name[]="WORDS";
void my_setup(){	// {{{
	error(F("test"));
	LAST=&top_head;
	DEBUG_DUMP(&f_words,"&f_words");
	DEBUG_DUMP(&top_head,"&top_head");
	DEBUG_DUMP(LAST,"LAST");
	
	xpHead1 temp_h=(xpHead1)HERE;
	*(ptr24_u*)HERE=V(P24p(LAST)); HERE+=3;		// 3B next ptr
	*HERE++=0;				// 1B attr
	uint8_t len=strlen_P(f_words_name);
	*HERE++=len;				// 1B len "words"
	strcpy_P(HERE,f_words_name); HERE+=len;// len Bytes (+\0, but we overwrite it next step)
	*(ptr24_u*)HERE=(ptr24_u){.u32=((uintptr_t)(&f_words)) * 2}; HERE+=3;	// codeword
	LAST=temp_h;

	error(F("my_setup"));
	IP.p24 = (CM ptr24_u *)&w_test_data;
	IP=V(IP);
	debug_dump(IP,F("IP\t"));
	DEBUG_DUMP(&f_docol,"&f_docol");
	push(0x21);
	print_words();
	f_next();
	pop();
	error(F("the end"));
	while(1){;};

}		// }}}
void my_loop() {};	// empty
