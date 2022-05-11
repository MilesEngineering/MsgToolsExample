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
        void Woken() override
        {
            debugPrintf("TC1 woken\n");
        }
};

// an example client that receives messages, and also prints time every 5s
class TestClient2 : public MessageClient
{
    public:
        TestClient2(MessagePool& pool, TestClient1& tc1)
        : MessageClient("tc2", &pool, 1000),
          m_tc1(tc1)
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
            debugPrintf("TC2 Waking TC1\n");
            m_tc1.Wake();
        }
    private:
        TestClient1& m_tc1;
};

int main (void)
{
    //# Note: Do not declare anything on the stack in main!
    //# FreeRTOS repurposes the main stack for ISRs once the scheduler starts,
    //# and that will corrupt any variables declared on the stack here.
    hw_init();

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
    static TestClient2 tc2(mp, tc1);
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
