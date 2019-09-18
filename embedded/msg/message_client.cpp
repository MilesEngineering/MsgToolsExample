#include "message_client.h"
#include "message_bus.h"

#include <iostream>
using namespace std;

extern "C" void client_callback( void* pvParameters );

MessageClient::MessageClient(const char* name, MessagePool* pool, int period, int priority, int stacksize)
: m_period(period),
  m_msgPool(pool),
  m_rxMsgs()
{
    BaseType_t xReturned;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
                    client_callback, /* Function that implements the task. */
                    name,            /* Text name for the task. */
                    stacksize,       /* Stack size in words, not bytes. */
                    (void*) this,    /* Parameter passed into the task. */
                    priority,        /* Priority at which the task is created. */
                    &m_taskHandle ); /* Used to pass out the created task's handle. */

    configASSERT( xReturned == pdPASS );
}
void MessageClient::RunLoop()
{
    // Store a pointer to ourself in the FreeRTOS thread-local storage array,
    // so CurrentClient() can return a pointer to us while we're executing.
    vTaskSetThreadLocalStoragePointer( m_taskHandle,  /* Task handle. */
                                       0,             /* Index into the array. */
                                       (void*)this ); /* The value to store. */
    m_lastIdleTime = xTaskGetTickCount();
    while(1)
    {
        TickType_t now = xTaskGetTickCount();
        TickType_t wait_time = m_lastIdleTime + m_period - now;
        //cout << "  waiting " << wait_time << " (" << m_lastIdleTime << " + " << m_period << " - " << now << ")" <<  endl;
        MessageBuffer* msgbuf = m_rxMsgs.get(wait_time);
        if(msgbuf)
        {
            Message msg(msgbuf);
            HandleReceivedMessage(msg);
        }
        else
        {
            m_lastIdleTime = xTaskGetTickCount();
            PeriodicTask();
        }
    }
}
void MessageClient::SendMessage(Message& msg)
{
    MessageBus::SendMessage(msg, this);
}
MessageClient* MessageClient::CurrentClient()
{
    MessageClient* c = (MessageClient*)pvTaskGetThreadLocalStoragePointer( nullptr, 0 );
    return c;
}
MessagePool* MessageClient::GetMessagePool()
{
    return m_msgPool;
}
void MessageClient::DeliverMessage(Message& msg)
{
    m_rxMsgs.put(msg.m_buf);
}
void MessageClient::PeriodicTask()
{
}
extern "C" void client_callback( void* pvParameters )
{
    MessageClient* c= (MessageClient*)pvParameters;
    c->RunLoop();
}
