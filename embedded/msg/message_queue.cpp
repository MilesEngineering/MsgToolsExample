#include "message_queue.h"

#include <iostream>
using namespace std;

MessageQueue::MessageQueue(int count)
{
    m_msgQueue = xQueueCreate(count, sizeof(void*));
    configASSERT( m_msgQueue );
}
MessageBuffer* MessageQueue::get(TickType_t waitTime)
{
    MessageBuffer* msgbuf=0;
    BaseType_t ret = xQueueReceive(m_msgQueue, (void*)&msgbuf, waitTime);
    if(ret && msgbuf)
    {
        //# can't decrement here, the caller needs to keep our reference!
        //msgbuf->decrement_refcount();
    }
    return msgbuf;
}
void MessageQueue::put(MessageBuffer* msgbuf)
{
    msgbuf->increment_refcount();
    BaseType_t ret = xQueueSend(m_msgQueue, (void*)&msgbuf, 0);
    configASSERT(ret == pdTRUE);
}
