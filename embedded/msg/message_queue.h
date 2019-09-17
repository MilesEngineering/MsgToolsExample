#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include "FreeRTOS.h"
#include "queue.h"
#include "message.h"

#define DEFAULT_QUEUE_LEN 16

class MessageQueue
{
    public:
        MessageQueue(int count=DEFAULT_QUEUE_LEN);
        MessageBuffer* get(TickType_t waitTime=0);
        void put(Message& msg);
    private:
        QueueHandle_t m_msgQueue;
};

#endif
