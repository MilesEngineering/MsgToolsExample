#include "FreeRTOS.h"
#include "tick.h"
#include "message_client.h"
#include "message_bus.h"
#include "message.h"
#include "message_pool.h"
#include "message_queue.h"
#include "debug_server.h"
#include "simulink/simulink_message_client.h"
#include "example_model_grt_rtw/example_model.h"
#include <math.h>
#include "hw_init.h"
#ifdef BUILD_SPEC_Linux
#include "Linux/network_client.h"
#else
#include "sam/serial_client.h"
#include "sam/can_client.h"
#endif
#include <inttypes.h>
#include "debug_printf.h"
#define ESCAPED_FILE_PATH __main

// Make a pool of buffers to be used by all clients.
#ifdef MSG_REFERENCE_COUNTING
#define POOL_BUF_COUNT    16
#else
#define POOL_BUF_COUNT    32
#endif
//# This is way bigger than CAN-FD packet size.
//# Either shrink it to 64, or implement fragmentation/reassembly on CAN-FD.
#define POOL_BUF_SIZE (128)

MessagePoolWithStorage<POOL_BUF_SIZE, POOL_BUF_COUNT> mp;

int main (void)
{
    //# Note: Do not declare anything on the stack in main!
    //# FreeRTOS repurposes the main stack for ISRs once the scheduler starts,
    //# and that will corrupt any variables declared on the stack here.
    hw_init();

    printf("\n\nMsgTools simulink example application.\n\n");
    
    MessagePool::SetInterruptContextPool(mp);
    static DebugServer dbs("SimulinkExample");
    static SimulinkMessageClient sc
    (mp,
     (SimulinkMessageClient::CreateModelFunctionT)&example_model,
     (SimulinkMessageClient::ModelFunctionT)&example_model_initialize,
     (SimulinkMessageClient::ModelFunctionT)&example_model_step);
#ifdef BUILD_SPEC_Linux
    static NetworkClient nc(mp);
    // CanClient works on Linux, but cannot be used at the same time as NetworkClient,
    // or an infinite loop of messages will be created.
    //static CanClient* can = CanClient::Can1(&mp);
#else
    //static SerialClient* sc = UsartClient::Usart0(&mp);
    static UsbCdcClient* usb = UsbCdcClient::Instance(&mp);
    static CanClient* can = CanClient::Can1(&mp);
#endif
    vTaskStartScheduler();
    return 0;
}
