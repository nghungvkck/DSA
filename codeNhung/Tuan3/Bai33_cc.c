// logic, bấm 1 hiển thị 1
// bấm 2 hiển thị 2
// bấm 3 hiển thị 3

#include <io.h>
#include <delay.h>

int mode = 0; // Biến lưu trạng thái: 0=Tắt, 1=Hiện số 1, 2=Hiện số 2, 3=Hiện số 3

// --- KHỐI XUẤT DỮ LIỆU RA LED ---
void hienthi(int i, unsigned char select_mask) {
    unsigned char f_data;
    PORTA = 0x00; PORTD = 0x00; f_data = 0x00; // Xóa dữ liệu cũ tránh bóng mờ
    
    // Gán mã hiển thị (PORTA, PORTD) và mã chọn LED (f_data) tùy theo số i
    if (i == 1) { f_data = 0x20; PORTA = 0x80; PORTD = 0x00; }
    if (i == 2) { f_data = 0x22; PORTA = 0x00; PORTD = 0x0E; }
    if (i == 3) { f_data = 0x22; PORTA = 0x80; PORTD = 0x0A; }
    
    PORTF = f_data | select_mask; // Kết hợp xuất ra PORTF để quyết định LED nào sáng
}

// --- KHỐI CẬP NHẬT HIỂN THỊ THEO TRẠNG THÁI ---
void updateDisplay() {
    if (mode == 1) hienthi(1, 0x58);
    else if (mode == 2) hienthi(2, 0x51);
    else if (mode == 3) hienthi(3, 0x49);
    else { PORTF = 0x59; PORTA = 0x00; PORTD = 0x00; } // Mode 0 thì tắt hết
}

// --- KHỐI CHƯƠNG TRÌNH CHÍNH ---
void main() {
    // Cấu hình chân: A, D, F là Output; B là Input
    DDRF = 0xFF; 
    DDRD = 0xFF; 
    DDRA = 0xFF;
    DDRB = 0x00; 
    PORTB = 0x0D; // Bật trở kéo lên cho nút bấm
    
    while (1) {
        // --- KHỐI XỬ LÝ NÚT BẤM BT1 (Tăng/Chọn Mode 1) ---
        if (PINB.2 == 0) { 
            delay_ms(20); // Chống dội
            if (PINB.2 == 0) { 
                mode = 1;
                while (PINB.2 == 0) updateDisplay(); // Giữ nút vẫn duy trì hiển thị
            } 
        }
        
        // --- KHỐI XỬ LÝ NÚT BẤM BT2 (Chọn Mode 2) ---
        if (PINB.3 == 0) { 
            delay_ms(20);
            if (PINB.3 == 0) { 
                mode = 2;
                while (PINB.3 == 0) updateDisplay(); 
            } 
        }
        
        // --- KHỐI XỬ LÝ NÚT BẤM BT3 (Chọn Mode 3) ---
        if (PINB.0 == 0) { 
            delay_ms(20);
            if (PINB.0 == 0) { 
                mode = 3;
                while (PINB.0 == 0) updateDisplay(); 
            } 
        }
        
        // --- KHỐI DUY TRÌ HIỂN THỊ LIÊN TỤC ---
        updateDisplay(); 
    }
}