# include <io.h>
# include <glcd.h>
# include <delay.h>
# include <font5x7.h>

void main(void){
    GLCDINIT_t glcd_init_data; 
    DDRD = 0x80;               
    PORTD = 0x80;              

    glcd_init_data.font = font5x7;         
    glcd_init_data.temp_coef = 80;      
    glcd_init_data.bias = 3;              
    glcd_init_data.vlcd = 50;              
    glcd_init(&glcd_init_data);            
    glcd_clear();                          

    glcd_outtextxy(10, 2, "Hello World"); 
    glcd_line(0, 12, 83, 12);             
    glcd_rectangle(5, 20, 20, 35);         
    glcd_circle(42, 28, 8);               
    glcd_line(65, 35, 75, 20);             
    glcd_line(75, 20, 85, 35);             
    glcd_line(65, 35, 85, 35);             
    while (1){}                            
}