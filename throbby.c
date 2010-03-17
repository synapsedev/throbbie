#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>

#define BITS_ON(PORT, PIN)     ((PORT) |=  (PIN))
#define BITS_OFF(PORT, PIN)    ((PORT) &= ~(PIN))
#define BITS_TOGGLE(PORT, PIN) ((PORT) ^=  (PIN))


static const uint8_t sinTable256[256] PROGMEM= {
0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 2, 2, 2, 2, 2, 
2, 2, 2, 2, 2, 2, 2, 2, 
2, 2, 3, 3, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 4, 4, 
4, 4, 4, 4, 4, 4, 4, 5, 
5, 5, 5, 5, 5, 5, 5, 6, 
6, 6, 6, 6, 6, 7, 7, 7, 
7, 7, 7, 8, 8, 8, 8, 8, 
9, 9, 9, 9, 9, 10, 10, 10, 
10, 11, 11, 11, 11, 12, 12, 12, 
13, 13, 13, 13, 14, 14, 14, 15, 
15, 15, 16, 16, 17, 17, 17, 18, 
18, 19, 19, 20, 20, 20, 21, 21, 
22, 22, 23, 23, 24, 24, 25, 26, 
26, 27, 27, 28, 29, 29, 30, 31, 
31, 32, 33, 34, 34, 35, 36, 37, 
38, 38, 39, 40, 41, 42, 43, 44, 
45, 46, 47, 48, 49, 50, 51, 52, 
54, 55, 56, 57, 58, 60, 61, 62, 
64, 65, 67, 68, 70, 71, 73, 75, 
76, 78, 80, 81, 83, 85, 87, 89, 
91, 93, 95, 97, 99, 101, 104, 106, 
108, 111, 113, 116, 118, 121, 123, 126, 
129, 132, 135, 138, 141, 144, 147, 150, 
154, 157, 161, 164, 168, 171, 175, 179, 
183, 187, 191, 195, 200, 204, 209, 213, 
218, 223, 228, 233, 238, 243, 249, 254, 
};


void init_ports() {
    DDRB |= 0x03;
    PORTB &= ~(0x03);
}

void init_timer() {
    TCCR0A = 0x00;
    TCCR0B = 0x02;
    TCNT0 = 0x00;
    OCR0A = 0x50;
    OCR0B = 0x00;
    TIMSK0 = (1<<2);
    TIFR0 = 0x00;
}


void next(uint8_t* r, uint8_t* g, uint8_t* b) {
	static uint8_t r_index = 0;
	static uint8_t g_index = 128;
	static uint8_t b_index = 0;
/*	
	*r = sinTable256[r_index++];
    *g = sinTable256[g_index++];
	*b = sinTable256[b_index++];
*/
    *r = pgm_read_byte(&sinTable256[r_index++]);
    *g = pgm_read_byte(&sinTable256[g_index++]);
    *b = pgm_read_byte(&sinTable256[b_index++]);
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
		static uint8_t j =0;

		if(j++ > 1){
			next(&r, &g, &b);
			j = 0;
		}
	}

    PORTB = (new_portb ^ 0x07);
	TCNT0 = 0;
}

int main(void) {
    //DDxn, PORTxn, and PINxn
	MCUSR &= ~(0x08);
    WDTCR = 0;
    init_ports();
    init_timer();


    sei();

    for(;;) {

    }
}
