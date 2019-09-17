#ifndef MESSAGE_POOL_H
#define MESSAGE_POOL_H

#include "message.h"
#include "FreeRTOS.h"
#include "queue.h"

// a pool of messages that can be allocated and freed.
class MessagePool
{
    public:
        MessagePool(uint8_t* buffer, int count, int size);
        static MessagePool* CurrentPool();
        MessageBuffer* Allocate(int size);
        void Free(MessageBuffer* msg);
    private:
        QueueHandle_t m_freeList;
};

#endif
