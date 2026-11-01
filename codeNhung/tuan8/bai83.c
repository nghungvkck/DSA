// Tram thoi tiet mini: DHT11 + LDR + GLCD 128x64
// - Doc nhiet do/do am tu DHT11
// - Doc anh sang qua ADC (LDR), phan loai Nang/Sang/Toi
// - Suy luan thoi tiet don gian tu T + H
// - Hien thi 4 dong + icon chu o goc phai

#include <io.h>
#include <delay.h>
#include <glcd.h>
#include <string.h>
#include <font5x7.h>

#define PIN_DHT  PINB.0
#define PORT_DHT PORTB.0
#define DDR_DHT  DDRB.0

unsigned char nhietdo, doam;
unsigned int adc_value;

// Doc ADC 10-bit tu kenh chi dinh (0..7)
unsigned int read_adc(unsigned char adc_input) {
    ADMUX = (adc_input & 0x1F) | 0x40;   // REFS=01 (AVCC), canh phai
    delay_us(10);                        // cho mux on dinh
    ADCSRA |= 0x40;                      // bat dau chuyen doi
    while ((ADCSRA & 0x10) == 0);        // cho ADIF
    ADCSRA |= 0x10;                      // xoa co
    return ADCW;
}

// Doc DHT11: tra ve 1 neu OK, 0 neu loi
char dht11(unsigned char *t, unsigned char *rh) {
    unsigned char i, j, sum;
    unsigned char buf[5] = {0, 0, 0, 0, 0};

    // Gui START: high 60us -> low 20ms
    DDR_DHT = 1;
    PORT_DHT = 1;
    delay_us(60);
    PORT_DHT = 0;
    delay_ms(20);

    // Tha chan sang input de nhan phan hoi
    PORT_DHT = 1;
    DDR_DHT = 0;
    delay_us(40);

    // Kiem tra phan hoi 80us low + 80us high
    if (PIN_DHT) return 0;
    while (!PIN_DHT);
    delay_us(60);
    if (!PIN_DHT) return 0;

    // Doc 40 bit: bit 0 (~28us high), bit 1 (~70us high)
    // Delay 40us sau suon len de phan biet
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

    // Checksum
    sum = buf[0] + buf[1] + buf[2] + buf[3];
    if (sum != buf[4]) return 0;

    *rh = buf[0];   // do am
    *t  = buf[2];   // nhiet do
    return 1;
}

// Ve icon thoi tiet bang chu cai o goc phai: O=nang, C=may, R=mua
void display_weather_icon(unsigned char weather) {
    if      (weather == 0) glcd_outtextxy(80, 20, "O");
    else if (weather == 1) glcd_outtextxy(80, 20, "C");
    else if (weather == 2) glcd_outtextxy(80, 20, "R");
}

void main(void) {
    char dht;
    char light_level[10];       // "Nang" / "Sang" / "Toi"
    char weather_str[10];       // "Sunny" / "Cloudy" / "Rainy"
    unsigned char weather = 0;  // 0=Nang, 1=May, 2=Mua
    GLCDINIT_t glcd_init_data;

    DDRD  = 0x80;
    PORTD = 0x80;   // bat den nen GLCD

    // Khoi tao GLCD 128x64
    glcd_init_data.font = font5x7;
    glcd_init_data.temp_coef = 63;
    glcd_init_data.bias = 4;
    glcd_init_data.vlcd = 50;
    glcd_init(&glcd_init_data);
    glcd_clear();

    // Khoi tao ADC: bat, prescaler /128
    ADMUX  = 0x40;
    ADCSRA = 0x87;
    SFIOR  = 0x00;

    while (1) {
        // Doc DHT11, lap den khi thanh cong
        do { dht = dht11(&nhietdo, &doam); }
        while (!dht);

        // Doc anh sang qua ADC kenh 0
        adc_value = read_adc(0);

        // Phan loai anh sang theo nguong ADC
        if      (adc_value > 700) strcpy(light_level, "Nang");
        else if (adc_value > 300) strcpy(light_level, "Sang");
        else                      strcpy(light_level, "Toi");

        // Suy luan thoi tiet don gian:
        //   nong + kho  -> Nang
        //   rat am      -> Mua
        //   con lai     -> May
        if (nhietdo > 30 && doam < 50) {
            weather = 0;
            strcpy(weather_str, "Sunny");
        }
        else if (doam > 70) {
            weather = 2;
            strcpy(weather_str, "Rainy");
        }
        else {
            weather = 1;
            strcpy(weather_str, "Cloudy");
        }

        // Hien thi 4 dong len GLCD
        glcd_clear();
        glcd_outtextxy(0, 0, "Anh sang: ");
        glcd_outtext(light_level);

        glcd_outtextxy(0, 10, "Nhiet do: ");
        glcd_putchar(nhietdo / 10 + '0');
        glcd_putchar(nhietdo % 10 + '0');
        glcd_outtext("oC");

        glcd_outtextxy(0, 20, "Do am: ");
        glcd_putchar(doam / 10 + '0');
        glcd_putchar(doam % 10 + '0');
        glcd_outtext("%");

        glcd_outtextxy(0, 30, "Thoi tiet: ");
        glcd_outtext(weather_str);

        display_weather_icon(weather);   // icon goc phai
        delay_ms(1000);
    }
}