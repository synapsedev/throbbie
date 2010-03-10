#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>

#define BITS_ON(PORT, PIN)     ((PORT) |=  (PIN))
#define BITS_OFF(PORT, PIN)    ((PORT) &= ~(PIN))
#define BITS_TOGGLE(PORT, PIN) ((PORT) ^=  (PIN))


static const uint8_t sinTable256[256] PROGMEM= {
63, 
62, 62, 62, 62, 62, 62, 62, 62, 
62, 62, 61, 61, 61, 61, 60, 60, 
60, 59, 59, 59, 58, 58, 58, 57, 
57, 56, 56, 55, 55, 54, 54, 53, 
53, 52, 51, 51, 50, 50, 49, 48, 
48, 47, 46, 46, 45, 44, 44, 43, 
42, 41, 41, 40, 39, 38, 38, 37, 
36, 35, 35, 34, 33, 32, 32, 31, 
30, 29, 28, 28, 27, 26, 25, 25, 
24, 23, 22, 22, 21, 20, 19, 19, 
18, 17, 17, 16, 15, 15, 14, 13, 
13, 12, 11, 11, 10, 10, 9, 9, 
8, 7, 7, 6, 6, 6, 5, 5, 
4, 4, 3, 3, 3, 2, 2, 2, 
1, 1, 1, 1, 1, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 
0, 1, 1, 1, 1, 1, 2, 2, 
2, 3, 3, 3, 4, 4, 5, 5, 
6, 6, 6, 7, 7, 8, 9, 9, 
10, 10, 11, 11, 12, 13, 13, 14, 
15, 15, 16, 17, 17, 18, 19, 19, 
20, 21, 22, 22, 23, 24, 25, 25, 
26, 27, 28, 28, 29, 30, 31, 32, 
32, 33, 34, 35, 35, 36, 37, 38, 
38, 39, 40, 41, 41, 42, 43, 44, 
44, 45, 46, 46, 47, 48, 48, 49, 
50, 50, 51, 51, 52, 53, 53, 54, 
54, 55, 55, 56, 56, 57, 57, 58, 
58, 58, 59, 59, 59, 60, 60, 60, 
61, 61, 61, 61, 62, 62, 62, 62, 
62, 62, 62, 62, 62, 62, 62, };


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
