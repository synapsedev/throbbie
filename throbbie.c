#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/common.h>
#include <avr/sleep.h>
#include <avr/pgmspace.h>

#define BITS_ON(PORT, PIN)     ((PORT) |=  (PIN))
#define BITS_OFF(PORT, PIN)    ((PORT) &= ~(PIN))
#define BITS_TOGGLE(PORT, PIN) ((PORT) ^=  (PIN))


static const uint8_t sinTable256[256] PROGMEM= {
 10,  10,  10,  10,  10,  10,  10,  10, 
 10,  11,  11,  11,  11,  11,  11,  11, 
 11,  11,  11,  11,  11,  12,  12,  12, 
 12,  12,  12,  12,  12,  13,  13,  13, 
 13,  13,  13,  14,  14,  14,  14,  14, 
 15,  15,  15,  16,  16,  16,  16,  17, 
 17,  17,  18,  18,  19,  19,  20,  20, 
 20,  21,  21,  22,  23,  23,  24,  24, 
 25,  26,  27,  27,  28,  29,  30,  31, 
 32,  33,  34,  35,  36,  37,  39,  40, 
 41,  43,  44,  46,  47,  49,  51,  53, 
 55,  57,  59,  61,  63,  66,  68,  71, 
 73,  76,  79,  82,  85,  89,  92,  96, 
100, 104, 108, 112, 117, 122, 127, 132, 
137, 143, 149, 155, 161, 168, 175, 182, 
190, 198, 206, 215, 224, 234, 244, 254, 
244, 234, 224, 215, 206, 198, 190, 182, 
175, 168, 161, 155, 149, 143, 137, 132, 
127, 122, 117, 112, 108, 104, 100,  96, 
 92,  89,  85,  82,  79,  76,  73,  71, 
 68,  66,  63,  61,  59,  57,  55,  53, 
 51,  49,  47,  46,  44,  43,  41,  40, 
 39,  37,  36,  35,  34,  33,  32,  31, 
 30,  29,  28,  27,  27,  26,  25,  24, 
 24,  23,  23,  22,  21,  21,  20,  20, 
 20,  19,  19,  18,  18,  17,  17,  17, 
 16,  16,  16,  16,  15,  15,  15,  14, 
 14,  14,  14,  14,  13,  13,  13,  13, 
 13,  13,  12,  12,  12,  12,  12,  12, 
 12,  12,  11,  11,  11,  11,  11,  11, 
 11,  11,  11,  11,  11,  11,  10,  10, 
 10,  10,  10,  10,  10,  10,  10,  10
};


void init_ports() {
    DDRB |= 0x03;
    PORTB &= ~(0x03);
}

void init_timer() {
    TCCR0A = 0x00;
    TCCR0B = 0x02;
    TCNT0 = 0x00;
    OCR0A = 0x08;
    OCR0B = 0x00;
    TIMSK0 = (1<<2);
    TIFR0 = 0x00;
}


void next(uint8_t* r, uint8_t* g, uint8_t* b) {
	static uint8_t r_index = 0;
	static uint8_t g_index = 128;
	static uint8_t b_index = 0;

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

	MCUSR &= ~(0x08);
    WDTCR = 0;
    init_ports();
    init_timer();

    sei();

    for(;;) {

    }
}
