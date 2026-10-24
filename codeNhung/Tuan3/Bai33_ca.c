// logic, bấm 1 hiển thị 1
// Dành cho CodeVisionAVR - LED 7 thanh ANODE CHUNG (Mức 0 sáng)

#include <io.h>
#include <delay.h>

int mode = 0; // 0=Tắt, 1=Hiện số 1, 2=Hiện số 2, 3=Hiện số 3

// --- KHỐI XUẤT DỮ LIỆU RA LED (ANODE CHUNG) ---
void hienthi(int i, unsigned char select_mask) {
    unsigned char f_data;
    
    // Xóa dữ liệu cũ tránh bóng mờ (Đưa tất cả về mức 1 vì là Anode chung)
    PORTA = 0xFF; 
    PORTD = 0xFF; 
    f_data = 0xFF; 
    
    // Mã hiển thị cho LED 7 thanh Anode chung (Mức 0 = sáng)
    // i == 1: hiển thị số 1 (đoạn b, c sáng)
    if (i == 1) { f_data = 0xDF; PORTA = 0x7F; PORTD = 0xFF; } 
    
    // i == 2: hiển thị số 2 (đoạn a, b, d, e, g sáng)
    if (i == 2) { f_data = 0xDD; PORTA = 0xFF; PORTD = 0xF1; } 
    
    // i == 3: hiển thị số 3 (đoạn a, b, c, d, g sáng)
    if (i == 3) { f_data = 0xDD; PORTA = 0x7F; PORTD = 0xF5; } 
    
    // Xuất ra PORTF để quyết định LED nào sáng (kết hợp với select_mask)
    PORTF = f_data & select_mask; 
}

// --- KHỐI CẬP NHẬT HIỂN THỊ THEO TRẠNG THÁI ---
void updateDisplay() {
    if (mode == 1) hienthi(1, 0x58);
    else if (mode == 2) hienthi(2, 0x51);
    else if (mode == 3) hienthi(3, 0x49);
    else { 
        // Mode 0 thì tắt hết (Đưa về mức 1)
        PORTF = 0xFF; 
        PORTA = 0xFF; 
        PORTD = 0xFF; 
    } 
}

// --- KHỐI CHƯƠNG TRÌNH CHÍNH ---
void main() {
    // Cấu hình chân: A, D, F là Output; B là Input
    DDRF = 0xFF; 
    DDRD = 0xFF; 
    DDRA = 0xFF;
    DDRB = 0x00; 
    PORTB = 0x0D; // Bật trở kéo lên cho nút bấm
    
    // Khởi tạo trạng thái ban đầu (tắt hết)
    PORTF = 0xFF;
    PORTA = 0xFF;
    PORTD = 0xFF;

    while (1) {
        // --- KHỐI XỬ LÝ NÚT BẤM BT1 (Chọn Mode 1) ---
        if (PINB.2 == 0) { 
            delay_ms(20); // Chống dội
            if (PINB.2 == 0) { 
                mode = 1;
                while (PINB.2 == 0) updateDisplay(); 
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