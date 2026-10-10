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
    char temp_base, hum_base;
    char temp_lower, temp_upper, hum_lower, hum_upper;
    char dht;
    DDRC = 0x0F;
    PORTC = 0x00;
    DDRD = 0x80;
    PORTD = 0x80;
    lcd_init(16);
    lcd_clear();
    do {
        dht = dht11(&temp_base, &hum_base);
    } while (!dht);
    temp_lower = temp_base - 2;
    temp_upper = temp_base + 2;
    hum_lower = hum_base - 2;
    hum_upper = hum_base + 2;
    while (1) {
        dht = dht11(&nhietdo, &doam);
        if (nhietdo > temp_upper) PORTC.4=1;
        else PORTC.4=0;
        if (nhietdo < temp_lower) PORTC.3 = 1;
        else PORTC.3 = 0;
        if (doam > hum_upper) PORTC.2 = 1;
        else PORTC.2 = 0;
        if (doam < hum_lower) PORTC.1 = 1;
        else PORTC.1= 0;
        lcd_gotoxy(0, 0);
        lcd_puts("T:");
        lcd_putchar(nhietdo / 10 + '0');
        lcd_putchar(nhietdo % 10 + '0');
        lcd_putchar('*');
        lcd_puts(" (");
        lcd_putchar(temp_lower / 10 + '0');
        lcd_putchar(temp_lower % 10 + '0');
        lcd_puts("-");
        lcd_putchar(temp_upper / 10 + '0');
        lcd_putchar(temp_upper % 10 + '0');
        lcd_puts(")");
        lcd_gotoxy(0, 1);
        lcd_puts("H:");
        lcd_putchar(doam / 10 + '0');
        lcd_putchar(doam % 10 + '0');
        lcd_putchar('%');
        lcd_puts(" (");
        lcd_putchar(hum_lower / 10 + '0');
        lcd_putchar(hum_lower % 10 + '0');
        lcd_puts("-");
        lcd_putchar(hum_upper / 10 + '0');
        lcd_putchar(hum_upper % 10 + '0');
        lcd_puts(")");
        delay_ms(1000);
    }
}