#include "message_pool.h"

#include "queue.h"
#include "message_client.h"

#include <iostream>
using namespace std;

MessagePool::MessagePool(uint8_t* buffer, int count, int size)
{
    m_freeList = xQueueCreate(count, sizeof(void*));
    configASSERT( m_freeList );
    
    for(int i=0; i<count; i++)
    {
        MessageBuffer* msg = (MessageBuffer*)&buffer[i*size];
#ifdef MSG_REFERENCE_COUNTING
        // set non-atomically before we put onto free list.
        // no one else can have a reference before that.
        msg->m_referenceCount = 0;
#endif
        msg->m_owner = this;
        msg->m_bufferSize = size - offsetof(MessageBuffer, m_data);
        Free(msg);
    }
    
    for(int i=0; i<count; i++)
    {
        MessageBuffer* buf = Allocate(0);
        Free(buf);
    }

    for(int i=0; i<count; i++)
    {
        MessageBuffer* buf = Allocate(0);
        Free(buf);
    }

}
MessagePool* MessagePool::CurrentPool()
{
    return MessageClient::CurrentClient()->GetMessagePool();
}
MessageBuffer* MessagePool::Allocate(int size)
{
    UNUSED(size);
    MessageBuffer* msg=0;
    xQueueReceive(m_freeList, (void*)&msg, 0);
#ifdef MSG_REFERENCE_COUNTING
    if(msg)
    {
        // set non-atomically after we get it from the free list and
        // before we return.
        // no one else can have a reference before that.
        msg->m_referenceCount = 0;
    }
#endif
    if(msg && size > msg->m_bufferSize)
    {
        Free(msg);
        return 0;
    }
    return msg;
}
void MessagePool::Free(MessageBuffer* msg)
{
#ifdef MSG_REFERENCE_COUNTING
    if (std::atomic_fetch_sub(&msg->m_referenceCount, 1) == 0)
#endif
    {
        BaseType_t ret = xQueueSend(m_freeList, (void*)&msg, 0);
        configASSERT(ret == pdTRUE);
    }
}
