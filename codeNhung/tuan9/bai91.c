#include <io.h>
#include <delay.h>
#include <i2c.h>
#include <ds1307.h>
char tableF[] = {
    0xFE, 0xFB, 0xFA, 0xFA, 0xDE, 0xDE, 0xFA, 0xFE, 0xFE, 0xFE};
char tableD[] = {
    0x06, 0x00, 0x0E, 0x0A, 0x08, 0x0A, 0x0E, 0x00, 0x0E, 0x0A};
char tableA[] = {
    0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80};
char pmask[] = {0xFE, 0xF7, 0xEF, 0xBF};
char h, m, s, i;
char digits[4];
void main(void) {
    DDRF = 0xFF; DDRD = 0xFF; DDRA = 0xFF;
    i2c_init();
    rtc_init(3, 1, 0);
    rtc_set_time(12, 59, 50);
    while (1) {
        rtc_get_time(&h, &m, &s);
        digits[0] = h / 10;
        digits[1] = h % 10;
        digits[2] = m / 10;
        digits[3] = m % 10;
        for (i = 0; i < 4; i++) {
            char tempF = tableF[digits[i]];
            if (s % 2 == 0) {tempF |= 0x80;}
            else {tempF &= ~0x7F;}
            tempF |= 0x59;
            PORTF = tempF & pmask[i];
            PORTD = tableD[digits[i]];
            PORTA = tableA[digits[i]];
            delay_ms(2);
            PORTF |= 0x59; 
        }
    }
}