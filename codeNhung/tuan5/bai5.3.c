// =========================
//  viết chương trình vẽ con lật đật
//  bấm bt_1 để con lật đật nhấp nháy,
//  bấm bt_2 con lật đật ở trạng thái ban đầu, đồng thời ngôi sao nhấp nháy
// =========================

# include <io.h>
# include <glcd.h>
# include <delay.h>
# include <font5x7.h>

# define BT1 PINB.2
# define BT2 PINB.3
# define ROY_X 30
# define ROY_Y 8
# define STAR_X 60
# define STAR_Y 20

flash unsigned char roly_left[] = {
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00
};

flash unsigned char roly_center[] = {
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00
};

flash unsigned char roly_right[] = {
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00
};

flash unsigned char star_bitmap[] = {
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00
};

void roly_show(flash unsigned char *img){
    glcd_clear();
    glcd_putimage(ROY_X, ROY_Y, img, 1);
}

void roly_sway_once(void){
    roly_show(roly_left); 
    delay_ms(500);
    roly_show(roly_center);
    delay_ms(500);
    roly_show(roly_right);
    delay_ms(500);
    roly_show(roly_center);
    delay_ms(500);
}

void blink_star(void){
    int i;
    for (i = 0; i < 3; i++){
        glcd_putimagef(STAR_X, STAR_Y, star_bitmap, 1);
        delay_ms(500);
        glcd_putimagef(STAR_X, STAR_Y, star_bitmap, 1);
        delay_ms(500);
    }
}

void main(void){
    GLCDINIT_t glcd_init_data;
    DDRD = 0x80;
    PORTD = 0x80;
    DDRB = 0x00;
    PORTB = 0x0C;

    glcd_init_data.font = font5x7;
    glcd_init_data.temp _coef = 80;
    glcd_init_data.bias = 3;
    glcd_init_data.vlcd = 65;
    glcd_init(&glcd_init_data);
    roly_show(roly_center);

    while (1){
        if (BT1 == 0){
            delay_ms(20);
            if (BT1 == 0){
                roly_sway_once();
                roly_sway_once();
                roly_show(roly_center);
                while (BT1 == 0);  
            }
        }

        if (BT2 == 0){
            delay_ms(20);
            if (BT2 == 0){
                roly_show(roly_center);
                blink_star();
                while (BT2 == 0){}
            }
        }
    }
}