#include <io.h>
#include <alcd.h>
#include <delay.h>
#define PIN_DHT PINB.0
#define PORT_DHT PORTB.0
#define DDR_DHT DDRB.0
unsigned char nhietdo, doam;
char dht11(unsigned char *t, unsigned char *rh) {
    unsigned char i, j, sum;
    unsigned char buf[5] = {0, 0, 0, 0, 0};
    DDR_DHT = 1;
    PORT_DHT = 1;
    delay_us(60);
    PORT_DHT = 0;
    delay_ms(20);
    PORT_DHT = 1;
    DDR_DHT = 0;
    delay_us(40);
    if (PIN_DHT) return 0;
    while (!PIN_DHT);
    delay_us(60);
    if (!PIN_DHT) return 0;
    while (PIN_DHT);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 8; j++) {
            while (!PIN_DHT);
            delay_us(40);
            if (PIN_DHT) {
                buf[i] |= (1 << (7 - j));
                while (PIN_DHT);
            }
            else {
                while (PIN_DHT);
            }
        }
    }
    sum = buf[0] + buf[1] + buf[2] + buf[3];
    if (sum != buf[4]) return 0;
    *rh = buf[0];
    *t = buf[2];
    return 1;
}
void main(void) {
    char dht;
    DDRD = 0x80;
    PORTD = 0x80;
    lcd_init(16);
    lcd_clear();
    while (1) {
        dht = dht11(&nhietdo, &doam);
        lcd_gotoxy(0, 0);
        lcd_puts("TEMP: ");
        lcd_putchar(nhietdo / 10 + '0');
        lcd_putchar(nhietdo % 10 + '0');
        lcd_putchar('*');
        lcd_puts("  ");
        lcd_gotoxy(0, 1);
        lcd_puts("HUM : ");
        lcd_putchar(doam / 10 + '0');
        lcd_putchar(doam % 10 + '0');
        lcd_putchar('%');
        lcd_puts("  ");
        delay_ms(1000);
    }
}