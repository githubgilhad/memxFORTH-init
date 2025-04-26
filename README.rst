memxFORTH-init
==============

A minimal Forth-like core for AVR (ATmega328P), designed to experiment with unified RAM and FLASH word handling
using `__memx` pointers in C. This project serves as a proof of concept and debugging tool base.
Based on `<https://arduino.stackexchange.com/questions/97384/how-to-manage-mixed-pointers-to-ram-and-progmem-on-arduino>`__

Project Goals
-------------
- Unified handling of dictionary words in both RAM and FLASH
- Explore low-footprint interpreter design in C
- Use `__memx` for flexible word list chaining and execution
- Minimal working example, not a full Forth implementation

Current Features
----------------
- Dictionary traversal using `__memx` pointers
- Execution of basic hardcoded words in FLASH and RAM
- Modular, small codebase suitable for embedded hardware debugging
- Uses C and AVR-GCC (also Arduino IDE compatible)

Build
-----

Compile as usual with `avr-gcc`, tested on ATmega328P.

**Arduino IDE:**

- Open `memxFORTH-init.ino`
- Select "Arduino Nano" with ATmega328P
- Upload as usual

Usage
-----
- Developed for ANSI terminal on a wide monitor
- Serial speed: 115200 baud
- Colors can be disabled in **colors.h**
- After upload, the demo:

  - Prints a header from the Arduino `.ino`
  - Builds the word `WORDS` in RAM
  - Prints memory debug dumps (green marks current position)
  - Pushes `0x21` to the data stack
  - Prints word list
  - Runs the word `TEST` (defined as `: TEST DOUBLE DEBUG ;`)
  - Note: press a key on each NEXT to continue
  - Traverses definitions (`: DOUBLE DUP + ;`) and computes the result
  - Uses a hacked `DEBUG` word to return to main code and end the demo
  - Pops and displays result from stack - see Notes

- Intended to verify whether `__memx` pointers can be used for seamless access to both FLASH and RAM word dictionaries
- RAM usage:

  - 3 variables
  - 10-item data stack
  - 10-item return stack
  - 100 bytes for user-defined words
  - Everything else resides in FLASH

- This project is mainly a source reference for other tools

Notes
-----
This is not meant to be a complete Forth interpreter. It is a technical foundation for later interactive tools, such as hardware pin testing, control, and diagnostics.

Fun detail:
  I threw `0x21` at my FORTH together with the word `: DOUBLE DUP + ;`
  
  and it left `0x42` on the stack - I’ll take that as a hexadecimal answer. :)

License
-------
GPL 2 or GPL 3 - choose the one that suits your needs.

Author
------
Gilhad - 2025
