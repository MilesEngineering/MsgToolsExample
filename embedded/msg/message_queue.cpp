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
    /*BaseType_t ret = */xQueueReceive(m_msgQueue, (void*)&msgbuf, waitTime);
    return msgbuf;
}
void MessageQueue::put(Message& msg)
{
    BaseType_t ret = xQueueSend(m_msgQueue, (void*)&msg.m_buf, 0);
    configASSERT(ret == pdTRUE);
}
