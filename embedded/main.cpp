#include "FreeRTOS.h"
#include "task.h"
#include "message_client.h"
#include "message_bus.h"
#include "message.h"
#include "message_pool.h"
#include "message_queue.h"
#include "TestCase2.h"
#include "TestCase4.h"
#include <math.h>

#include "console.h"

#ifdef BUILD_SPEC_Linux
#include "Linux/network_client.h"
#endif
#ifdef BUILD_SPEC_samv71
#include "sam/serial_client.h"
#include "sam/can_client.h"
#include "services/ioport/ioport.h"
#endif

#include <inttypes.h>

//# should probably move to a new file, tick.cpp
TickType_t GetTickCount()
{
    if(xPortIsInsideInterrupt())
    {
        return xTaskGetTickCountFromISR();
    }
    return xTaskGetTickCount();
}

// Make a pool of buffers to be used by all clients.
#ifdef MSG_REFERENCE_COUNTING
#define POOL_SIZE    4
#else
#define POOL_SIZE    16
#endif
//# This is way bigger than CAN-FD packet size.
//# Either shrink it to 64, or implement fragmentation/reassembly on CAN-FD.
#define POOL_BUF_LEN 128

uint8_t buf[POOL_SIZE*POOL_BUF_LEN];
MessagePool mp(buf, POOL_SIZE, POOL_BUF_LEN);


// An example client that sends a test messages periodically
class TestClient1 : public MessageClient
{
    public:
        TestClient1(MessagePool& pool)
        : MessageClient("tc1", &pool, 500)
        {
            MessageBus::Subscribe(this, TestCase4Message::MSG_ID);
            MessageBus::Subscribe(this, TestCase2Message::MSG_ID);
        }
        void HandleReceivedMessage(Message& msg)
        {
            UNUSED(msg);
#ifdef BUILD_SPEC_Linux
            printf("  TC1 got %" PRId32 " at time %" PRId32 "\n", msg.GetMessageID(), xTaskGetTickCount());
#endif
        }
        void PeriodicTask()
        {
            //printf("TC1.PeriodicTask()\n");
            TestCase4Message tcm;
            if(tcm.Exists())
            {
                tcm.SetA(xTaskGetTickCount());
#ifdef BUILD_SPEC_Linux
                printf("TC1 sending MessageID %" PRId32 " at time %" PRId32 "\n",tcm.GetMessageID(),xTaskGetTickCount());
#endif
                SendMessage(tcm);
            }
        }
};

// an example client that receives messages, and also prints time every 5s
class TestClient2 : public MessageClient
{
    public:
        TestClient2(MessagePool& pool)
        : MessageClient("tc2", &pool, 5000)
        {
            MessageBus::Subscribe(this, TestCase4Message::MSG_ID);
            MessageBus::Subscribe(this, TestCase2Message::MSG_ID);
        }
        void HandleReceivedMessage(Message& msg)
        {
#ifdef BUILD_SPEC_Linux
            switch(msg.GetMessageID())
            {
                case TestCase4Message::MSG_ID:
                    printf("  TC2 got TestCase4 Message  at time %" PRId32 "\n", xTaskGetTickCount());
                    break;
                default:
                    printf("  TC2 got %" PRId32 " at time %" PRId32 "\n", msg.GetMessageID(), xTaskGetTickCount());
                    break;
            }
#endif
        }
        void PeriodicTask()
        {
#ifdef BUILD_SPEC_Linux
            printf(">>>> TC2.PeriodicTask() at time %" PRId32 "\n", xTaskGetTickCount());
#endif
        }
};

int main (void)
{
    //# Note: Do not declare anything on the stack in main!
    //# FreeRTOS repurposes the main stack for ISRs once the scheduler starts,
    //# and that will corrupt any variables declared on the stack here.
#ifdef BUILD_SPEC_samv71
	/* Disable the watchdog */
	WDT->WDT_MR = WDT_MR_WDDIS;
    //# Unclear what may break if Instruction or Data caches are enabled.
    //# Lots of info here: https://www.avrfreaks.net/forum/same70-usb-cache
    //# Sounds like enabling D cache causes problems for any peripheral drivers
    //# that use DMA, which includes USB and SD card.  Apparently it also
    //# causes problems for MCAN which doesn't use DMA, but does have RAM
    //# buffers shared between the CPU and the peripheral.  Not sure which
    //# other drivers do, too.  Supposedly enabling I cache doesn't cause problems
    //# for peripherals, not sure if there's any downsides (like when examining
    //# memory via the debugger?).
    //# There's apparently some problems with incompatibility I cache and external
    //# memory, but that might be because the user configured DMA to access the
    //# external memory via SPI?!?  Also note some users had problems with I cache
    //# if they enabled it early in startup, and didn't have problems if they did
    //# so *after* all their peripheral initialization code!  That sounds scary and
    //# they might still have problems, just not immediately noticable ones.
    //# Latest ASF might have fixed D cache for USB and SD card, but not for CAN.
    //# Enabling D cache but then using MPU to specify that RAM buffers used by
    //# peripheral drivers should not be cached might work, but we'd have to make
    //# sure to do it for *all* necessary peripheral drivers!  If USB and SD card
    //# were fixed in latest ASF, then we wouldn't have to do it for them.
    //# Note that CONF_BOARD_ENABLE_CACHE and CONF_BOARD_ENABLE_CACHE_AT_INIT
    //# both exist and have different purposes.  The regular version is only used
    //# in example init code that decides to enable or disable the cache, and the
    //# AT_INIT version is used in the usbhs and xdmac peripherals to add cache
    //# invalidation function calls so the peripherals work with the D cache enabled.
    //# Also note that xdmac (despite it's generic sounding name is *only* used by
    //# hsmci (High Speed Memory Card Interface, for SD cards).
    //# There's nothing in the MCAN peripheral driver code about caching or DMA!
    //# Also need to research if D cache has any thread safety implications, for
    //# FreeRTOS data structures or message buffers.  Seems like it shouldn't
    //# because the memory is all accessed by the same CPU at the same addresses.
#ifdef CONF_BOARD_ENABLE_CACHE
	/* Enabling the Cache */
	SCB_EnableICache(); 
	SCB_EnableDCache();
#endif
	sysclk_init();
	ioport_init();
    configure_console(ID_USART1); // edbg uart on same70 eval board is USART1
#endif

    printf("\n\nMsgTools embedded example application.\n\n");
    
    MessagePool::SetInterruptContextPool(mp);
    static TestClient1 tc1(mp);
    static TestClient2 tc2(mp);
#ifdef BUILD_SPEC_Linux
    static NetworkClient nc(mp);
#endif
#ifdef BUILD_SPEC_samv71
    //static SerialClient* sc = UsartClient::Usart0(&mp);
    static UsbCdcClient* usb = UsbCdcClient::Instance(&mp);
    static CanClient* can = CanClient::Can1(&mp);
#endif
    MessageClient::InitializeAll();
    vTaskStartScheduler();
    return 0;
}

extern "C" void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
    printf("ASSERT: %s : %d\n", pcFileName, (int)ulLine);
    while(1);
}

extern "C" void vApplicationMallocFailedHook(void)
{
    printf("vApplicationMallocFailedHook\n");
	while(1);
}

extern "C" void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */

	/* Force an assert. */
	configASSERT( ( volatile void * ) NULL );
}

#ifdef BUILD_SPEC_samv71
extern "C" void HardFault_Handler(void)
{
    while(1) {
        #pragma GCC diagnostic ignored "-Wunused-variable"
        volatile uint32_t ipsr = __get_IPSR();
        #pragma GCC diagnostic ignored "-Wunused-variable"
        volatile uint32_t msp = __get_MSP();
        #pragma GCC diagnostic ignored "-Wunused-variable"
        volatile uint32_t psp = __get_PSP();
    }
}
#endif