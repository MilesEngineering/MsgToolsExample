#include "FreeRTOS.h"
#include "tick.h"
#include "message_client.h"
#include "message_bus.h"
#include "message.h"
#include "message_pool.h"
#include "message_queue.h"
#include "debug_server.h"
#include "TestCase2.h"
#include "TestCase4.h"
#include <math.h>
#ifdef BUILD_SPEC_Linux
#include "Linux/network_client.h"
#endif
#ifdef BUILD_SPEC_sam
#include "sam/console.h"
#include "sam/serial_client.h"
#include "sam/can_client.h"
#include "services/ioport/ioport.h"
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

// An example client that sends a test messages periodically
class TestClient1 : public MessageClient
{
    public:
        TestClient1(MessagePool& pool)
        : MessageClient("tc1", &pool, 500)
        {
            MessageBus::Subscribe(this, MessageKey(TestCase4Message::MSG_ID));
            MessageBus::Subscribe(this, MessageKey(TestCase2Message::MSG_ID));
        }
        void HandleReceivedMessage(Message& msg)
        {
            UNUSED(msg);
            debugPrintf("TC1 got %d at time %d\n", (int)msg.GetMessageID(), (int)GetTickCount());
        }
        void PeriodicTask()
        {
            //printf("TC1.PeriodicTask()\n");
            TestCase4Message tcm;
            if(tcm.Exists())
            {
                tcm.SetA(GetTickCount());
                debugPrintf("TC1 sending MessageID %d at time %d\n",(int)tcm.GetMessageID(),(int)GetTickCount());
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
            switch(msg.GetMessageID())
            {
                case TestCase4Message::MSG_ID:
                    debugPrintf("TC2 got TestCase4 Message  at time %d\n", (int)GetTickCount());
                    break;
                default:
                    debugPrintf("TC2 got %d at time %d\n", (int)msg.GetMessageID(), (int)GetTickCount());
                    break;
            }
        }
        void PeriodicTask()
        {
#ifdef BUILD_SPEC_Linux
            debugPrintf(">>>> TC2.PeriodicTask() at time %d\n", GetTickCount());
#endif
        }
};

int main (void)
{
    //# Note: Do not declare anything on the stack in main!
    //# FreeRTOS repurposes the main stack for ISRs once the scheduler starts,
    //# and that will corrupt any variables declared on the stack here.
#ifdef BUILD_SPEC_sam
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
    
    // Declare pool and clients here so their constructors happen at a predictable place
    // versus global objects being constructed before main starts.
    // Make them static so their memory usage is fixed in the memory map at build time,
    // and because FreeRTOS intentionally discards the contents of the stack of main
    // after vTaskStartScheduler().
    static MessagePoolWithStorage<POOL_BUF_SIZE, POOL_BUF_COUNT> mp;
    MessagePool::SetInterruptContextPool(mp);
    static DebugServer dbs("ExampleApp");
    static TestClient1 tc1(mp);
    static TestClient2 tc2(mp);
#ifdef BUILD_SPEC_Linux
    static NetworkClient nc(mp);
#endif
#ifdef BUILD_SPEC_sam
    //static SerialClient* sc = UsartClient::Usart0(&mp);
    static UsbCdcClient* usb = UsbCdcClient::Instance(&mp);
    static CanClient* can = CanClient::Can1(&mp);
#endif
    MessageClient::InitializeAll();
    vTaskStartScheduler();
    return 0;
}
