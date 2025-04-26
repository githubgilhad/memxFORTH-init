#include <avr/pgmspace.h>
extern "C" {
	extern void my_setup();
	extern void my_loop();
	char read_char() {
		if (Serial.available()) {
			return Serial.read();
		} else {
			return 0;
		}
	}

	void write_char(char c) {
		Serial.write(c);
	}
	void write_charA(char c) {
		Serial.write(c < ' ' ? '.' : c);
	}
}
void setup(){
	Serial.begin(115200); //	(115200/9600);
	while (!Serial){;};
	Serial.println(F("---- ==== #### FORTH #### ==== ----"));
	my_setup();
	Serial.println(F("Setup done"));
}

void loop(){
	my_loop();
}
