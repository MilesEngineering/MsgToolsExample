#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "message_client.h"
#include "message_bus.h"
#include "message.h"
#include "message_pool.h"
#include "message_queue.h"
#include "TestCase1.h"

#include "network_client.h"

#include <iostream>
using namespace std;

// Make a pool of buffers to be used by all clients.
#define POOL_BUF_LEN 128
#define POOL_SIZE    3
uint8_t buf[POOL_SIZE*POOL_BUF_LEN];
MessagePool mp(buf, POOL_SIZE, POOL_BUF_LEN);


// An example client that sends a TestCase1 message periodically
class TestClient1 : public MessageClient
{
    public:
        TestClient1(MessagePool& pool)
        : MessageClient("tc1", &pool, 1000)
        {
        }
        void HandleReceivedMessage(Message& msg)
        {
            UNUSED(msg);
        }
        void PeriodicTask()
        {
            //cout << "TC1.PeriodicTask()" << endl;
            TestCase1Message tc1;
            if(tc1.Exists())
            {
                tc1.SetFieldA(xTaskGetTickCount());
                cout << "TC1 sending MessageID " << tc1.GetMessageID() << " at time " << xTaskGetTickCount() << endl;
                SendMessage(tc1);
            }
        }
};

// an example client that receives TestCase1 messages, and also prints time every 5s
class TestClient2 : public MessageClient
{
    public:
        TestClient2(MessagePool& pool)
        : MessageClient("tc2", &pool, 5000)
        {
            MessageBus::Subscribe(this, TestCase1Message::MSG_ID);
        }
        void HandleReceivedMessage(Message& msg)
        {
            switch(msg.GetMessageID())
            {
                case TestCase1Message::MSG_ID:
                    cout << "  TC2 got TestCase1 Message" << " at time " << xTaskGetTickCount() << endl;
                    break;
                default:
                    cout << "  TC2 got " << msg.GetMessageID() << " at time " << xTaskGetTickCount() << endl;
                    break;
            }
        }
        void PeriodicTask()
        {
            cout << ">>>> TC2.PeriodicTask()" << " at time " << xTaskGetTickCount() << endl;
        }
};

int main (void)
{
    cout << "MsgTools embedded example application." << endl;

    TestClient1 tc1(mp);
    TestClient2 tc2(mp);
    NetworkClient nc(mp);
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
    cout << "vApplicationMallocFailedHook" << endl;
	while(1);
}


