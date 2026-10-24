// Hệ thống gồm 2 cảm biến: BT1 và BT2 đặt ở 2 bên cửa (hoặc 1 cảm biến ở cửa vào, 1 ở cửa ra)

// Đếm người VÀO (Đi từ ngoài vào trong):
// Khi có người đi qua: Họ sẽ chạm cảm biến BT1 trước (tín hiệu BT1 = 0).
// Sau đó một lúc ngắn (họ đi bộ tiếp), họ sẽ chạm cảm biến BT2 (tín hiệu BT2 = 0).
// Logic: Nếu thấy BT1 chạm trước, rồi sau đó BT2 chạm tiếp -> Xác nhận có 1 người đi VÀO -> Biến đếm tăng lên 1 (đóng Relay 1).

// Đếm người RA (Đi từ trong ra ngoài):
// Ngược lại, họ chạm BT2 trước, rồi mới chạm BT1.
// Logic: Nếu thấy BT2 chạm trước, rồi sau đó BT1 chạm tiếp -> Xác nhận có 1 người đi RA -> Biến đếm giảm xuống 1 (đóng Relay 2).
// Nếu kẹt ở giữa (Chạm 1 cảm biến quá lâu): 
// Nếu một cảm biến bị chạm (giữ) quá 4 nhịp đếm (mô phỏng việc có người đứng kẹt ở cửa), 
// hệ thống sẽ không đếm nữa mà chuyển sang trạng thái cảnh báo/giữ trạng thái (mở cả 2 Relay).

#include <mega128.h>
#include <delay.h>

// Khai báo chân kết nối nút bấm và relay
#define BT_2    PINB.3
#define BT_1    PINB.2
#define RL_1    PORTC.3
#define RL_2    PORTC.2

// Các biến toàn cục lưu trạng thái
unsigned char state = 0;
unsigned int dem = 0;

// Hàm cập nhật trạng thái cho các Relay dựa vào biến 'state'
void update_relays(void) {
    if (dem >= 11) { 
        RL_1 = 1; 
        RL_2 = 1; 
    } 
    else if (dem > 4) { 
        RL_2 = 1; 
    } 
    else { 
        RL_1 = 0; 
        RL_2 = 0; 
    }
}

// Hàm khởi tạo các cổng I/O
void io_init(void) {
    DDRB = 0x0C;    // Cấu hình chân PB2, PB3 là ngõ ra (Dựa theo sách in, tuy nhiên để đọc nút bấm thường để là 0x00)
    PORTB = 0x0C;   // Đặt mức cao cho PB2, PB3 (Bật điện trở kéo lên)
    DDRC = 0x0C;    // Cấu hình chân PC2, PC3 là ngõ ra để điều khiển Relay
    PORTC = 0x00;   // Ban đầu tắt cả 2 relay
    dem = 0;
    state = 0;
    update_relays();
}

// Hàm xử lý chính
void main(void) {
    io_init();
    
    while (1) {
        // --- XỬ LÝ NÚT BẤM 1 ---
        if (BT_1 == 0) { 
            delay_ms(20); // Chống dội phím
            if (BT_1 == 0) { 
                state = 1; 
            }
            
            // Vòng lặp đếm thời gian khi vẫn đang giữ nút BT_1
            while (BT_1 == 0) { 
                delay_ms(250); 
                state = 1; 
                if (BT_2 == 0) { 
                    delay_ms(20); 
                }
            } 
            dem = 0; // Reset biến đếm sau khi nhả nút
        }
        
        // --- XỬ LÝ NÚT BẤM 2 ---
        else if (BT_2 == 0) { 
            delay_ms(20); // Chống dội phím
            if (BT_2 == 0) { 
                state = 2; 
            }
            
            // Vòng lặp đếm thời gian khi vẫn đang giữ nút BT_2
            while (BT_2 == 0) { 
                delay_ms(20); 
                if (dem < 255) { 
                    dem++; 
                }
                update_relays(); 
                state = 0; 
            }
        }
        
        // --- XỬ LÝ TRẠNG THÁI (STATE) ---
        else { 
            if (state == 1) { 
                RL_1 = 1; 
            } 
            else if (state == 2) { 
                RL_2 = 1; 
            } 
            else { 
                RL_1 = 0; 
                RL_2 = 0; 
            }
        }
    }
}