#include "FreeRTOS.h"
#include "tick.h"
#include "message_client.h"
#include "message_bus.h"
#include "message.h"
#include "message_pool.h"
#include "message_queue.h"

#include "hw_init.h"
#ifdef BUILD_SPEC_Linux
#include "Linux/network_client.h"
#else
#include "serial_client.h"
#endif
#include "can_client.h"
#include <inttypes.h>
#include <cstdio>

#define POOL_BUF_COUNT    32

//# This is way bigger than CAN-FD packet size.
//# Either shrink it to 64, or implement fragmentation/reassembly on CAN-FD.
#define POOL_BUF_SIZE (128)

MessagePoolWithStorage<POOL_BUF_SIZE, POOL_BUF_COUNT> mp;

// On embedded hardware, this passes messages between CAN and USB.
// On PC, this passes messages between SocketCAN and msgserver.
// (Note that if you use msgserver's SocketCAN plugin, this will create an infinite loop,
//  so you should instead connect a physical USB-to-CAN adapter, and not use the msgserver CAN plugin).
int main (void)
{
    //# Note: Do not declare anything on the stack in main!
    //# FreeRTOS repurposes the main stack for ISRs once the scheduler starts,
    //# and that will corrupt any variables declared on the stack here.
    HWInit::hw_init();

    printf("\n\nMsgTools CAN-FD Gateway.\n\n");
    
    MessagePool::SetInterruptContextPool(mp);
#ifdef BUILD_SPEC_Linux
    static NetworkClient nc(mp);
#else
#ifdef BUILD_SPEC_samx7x
    static UsbCdcClient* usb = UsbCdcClient::Instance(&mp);
    static PinMode can1_rx_pin(PIO_PC12_IDX, IOPORT_MODE_MUX_C);
    static PinMode can1_tx_pin(PIO_PC14_IDX, IOPORT_MODE_MUX_C);
    [[maybe_unused]] static CanClient* can1 = CanClient::Can1(&can1_rx_pin, &can1_tx_pin, &mp);
#else // BUILD_SPEC_samx7x
#endif // BUILD_SPEC_samx7x
#endif
    

    vTaskStartScheduler();
    return 0;
}
