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
#include "network_client.h"
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

    printf("\n\nMsgTools embedded example application.\n\n");
    
    MessagePool::SetInterruptContextPool(mp);
    static TestClient1 tc1(mp);
    static TestClient2 tc2(mp);
#ifdef BUILD_SPEC_Linux
    static NetworkClient nc(mp);
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

