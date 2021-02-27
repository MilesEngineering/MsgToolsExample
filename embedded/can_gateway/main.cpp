#include "FreeRTOS.h"
#include "tick.h"
#include "message_client.h"
#include "message_bus.h"
#include "message.h"
#include "message_pool.h"
#include "message_queue.h"

#include "sam/console.h"
#include "sam/serial_client.h"
#include "sam/can_client.h"
#include "services/ioport/ioport.h"

#include <inttypes.h>

#define POOL_BUF_COUNT    32

//# This is way bigger than CAN-FD packet size.
//# Either shrink it to 64, or implement fragmentation/reassembly on CAN-FD.
#define POOL_BUF_SIZE (128)

MessagePoolWithStorage<POOL_BUF_SIZE, POOL_BUF_COUNT> mp;


int main (void)
{
    //# Note: Do not declare anything on the stack in main!
    //# FreeRTOS repurposes the main stack for ISRs once the scheduler starts,
    //# and that will corrupt any variables declared on the stack here.

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

    printf("\n\nMsgTools CAN-FD Gateway.\n\n");
    
    MessagePool::SetInterruptContextPool(mp);
    static UsbCdcClient* usb = UsbCdcClient::Instance(&mp);
    static CanClient* can = CanClient::Can1(&mp);
    MessageClient::InitializeAll();
    vTaskStartScheduler();
    return 0;
}
