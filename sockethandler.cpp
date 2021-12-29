#include "sockethandler.h"
/*typedef struct {
    uint8_t start;
    float x_pos;
    float y_pos;
    float theta;
} _attribute_((packed)) data_packet;

//_attribute_((packed))
void send_data_thread(void* par) {
    static data_packet msg;
    static XYTheta cur_pos;
    
    msg.start = 255;
    
    QueueHandle_t nano_q;
    // os_create_queue_dynamic(rxq, uint8_t, 64);
    os_create_queue_stack(nano_q, uint8_t, sizeof(msg)*10);
    // nano_q = xQueueCreate(32, 1);
    uart_init(par, 115200, 1);
    queued_uart_tx_init(par, nano_q);
    
    uint32_t last_ticks = get_ticks();
    
    // uint8_t arr[32];
    // data_packet* packet = (data_packet*)arr;
    
    while (1) {
        delay_until(last_ticks, 5);
        cur_pos = wb_current_position();
        
        msg.x_pos = cur_pos.x;
        msg.y_pos =  cur_pos.y;
        msg.theta = cur_pos.theta;
        
        queued_uart_tx(par, &msg, sizeof(msg));
        
        gpio_toggle(LED2);
    }
}*/
SocketHandler::SocketHandler()
{

}
