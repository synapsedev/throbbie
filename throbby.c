#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <avr/sleep.h>

#define BITS_ON(PORT, PIN)     ((PORT) |=  (PIN))
#define BITS_OFF(PORT, PIN)    ((PORT) &= ~(PIN))
#define BITS_TOGGLE(PORT, PIN) ((PORT) ^=  (PIN))


static int sinTable256[256] = {
127, 
130, 133, 136, 140, 143, 146, 149, 152, 
155, 158, 161, 164, 167, 170, 173, 176, 
179, 182, 185, 187, 190, 193, 195, 198, 
201, 203, 206, 208, 211, 213, 215, 217, 
220, 222, 224, 226, 228, 230, 232, 233, 
235, 237, 238, 240, 241, 242, 244, 245, 
246, 247, 248, 249, 250, 251, 252, 252, 
253, 253, 254, 254, 254, 254, 254, 254, 
254, 254, 254, 254, 253, 253, 252, 252, 
251, 250, 250, 249, 248, 247, 246, 244, 
243, 242, 240, 239, 237, 236, 234, 232, 
231, 229, 227, 225, 223, 221, 219, 216, 
214, 212, 209, 207, 204, 202, 199, 197, 
194, 191, 189, 186, 183, 180, 177, 175, 
172, 169, 166, 163, 160, 157, 154, 150, 
147, 144, 141, 138, 135, 132, 129, 125, 
122, 119, 116, 113, 110, 107, 104, 100, 
97, 94, 91, 88, 85, 82, 79, 77, 
74, 71, 68, 65, 63, 60, 57, 55, 
52, 50, 47, 45, 42, 40, 38, 35, 
33, 31, 29, 27, 25, 23, 22, 20, 
18, 17, 15, 14, 12, 11, 10, 8, 
7, 6, 5, 4, 4, 3, 2, 2, 
1, 1, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 1, 1, 2, 2, 
3, 4, 5, 6, 7, 8, 9, 10, 
12, 13, 14, 16, 17, 19, 21, 22, 
24, 26, 28, 30, 32, 34, 37, 39, 
41, 43, 46, 48, 51, 53, 56, 59, 
61, 64, 67, 69, 72, 75, 78, 81, 
84, 87, 90, 93, 96, 99, 102, 105, 
108, 111, 114, 118, 121, 124, 127 };


void init_ports() {
    DDRB |= 0x03;
    PORTB &= ~(0x03);
}

void init_timer() {
    TCCR0A = 0x00;
    TCCR0B = 0x02;
    TCNT0 = 0x00;
    OCR0A = 0x40;
    OCR0B = 0x00;
    TIMSK0 = (1<<2);
    TIFR0 = 0x00;
}


void next(int* r, int* g, int* b) {
	static uint8_t b_index = 0;
	
	*b = sinTable256[b_index++];
}


ISR(TIM0_COMPA_vect) {
    static uint8_t r = 128;
    static uint8_t g = 64;
    static uint8_t b = 32;
    static uint8_t count;
    count++;
    uint8_t new_portb = 0;

    if(count < r) {
        new_portb |= 0x04;
    }

    if(count < g) {
        new_portb |= 0x02;
    }

    if(count < b) {
        new_portb |= 0x01;
    }

	if(count == 0) {
		next(&r, &g, &b);
	}

    PORTB = new_portb;
	TCNT0 = 0;
}

int main(void) {
    //DDxn, PORTxn, and PINxn
    init_ports();
    init_timer();


    sei();

    for(;;) {

    }
}
