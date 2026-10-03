// ==============================
// viết chương trình thực hiện quét bàn phím ma trận 3x3 để phát hiện phím bấm
// và hiển thị lên LCD 16x2, khi người dùng bấm 1 phím bất kì, LCD hiện thị số của phím tương ứng 
// đồng thời hiển thị thêm chỉ số hàng (ROW) và cột (COL) của phím đó để phục vụ kiểm tra và giair mã 
// ==============================

# include <io.h>
# include <delay.h>
# include <stdio.h>
# include <alcd.h>

int keypad[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};

char data [16];
void show_key(char key, char row, char col){
    lcd_clear();
    lcd_gotoxy(0, 0);
    sprintf(data, "Key: %d", key);
    lcd_puts(data);
    lcd_gotoxy(0, 1);
    sprintf(data, "Row: %d, Col: %d", row, col);
    lcd_puts(data);
}

void button(void){
    char r;
    for (r = 0; r < 3; r++){
        if (r== 0){
            PORTC = 0X30;
            PORTE = 0X03;
            PORTD = 0X81;
        }

        if (r== 1){
            PORTC = 0X70;
            PORTE = 0X01;
            PORTD = 0X82;
        }

        if (r== 2){
            PORTC = 0X70;
            PORTE = 0X03;
            PORTD = 0X80;
        }
        delay_us(10);
        if  ((PINC & 0X10) == 0){
            Delay_ms(20);
            if ((PINC & 0X10) == 0){
                show_key(keypad[r][0], (char)(r+1), 1);
                while ((PINC & 0X10) == 0) delay_ms(10);
                return;
            }
        }

        if  ((PINC & 0X20) == 0){
            Delay_ms(20);
            if ((PINC & 0X20) == 0){
                show_key(keypad[r][1], (char)(r+1), 2);
                while ((PINC & 0X20) == 0) delay_ms(10);
                return;
            }
        }

        if  ((PINC & 0X01) == 0){
            Delay_ms(20);
            if ((PINC & 0X01) == 0){
                show_key(keypad[r][2], (char)(r+1), 3);
                while ((PINC & 0X01) == 0) delay_ms(10);
                return;
            }
        }
    }
}

void main(void){
    lcd_init(16);
    lcd_clear();
    lcd_gotoxy(0, 0);
    lcd_puts("Press keypad.....");
    lcd_gotoxy(0, 1);
    lcd_putsf("show key, row, col");
    DDRC = 0X40;
    PORTC = 0x70;
    DDRE = 0X02;
    PORTE = 0X03;
    DDRD = 0X81;
    PORTD = 0X81;
    while (1)
    {
        button();
    } 
}