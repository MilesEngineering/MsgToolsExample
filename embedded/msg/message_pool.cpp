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
        std::atomic_store(&msg->m_referenceCount, 1);
#endif
        msg->m_owner = this;
        msg->m_bufferSize = size - offsetof(MessageBuffer, m_data);
        Free(msg);
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
    //int old_len = uxQueueMessagesWaiting(m_freeList);
    xQueueReceive(m_freeList, (void*)&msg, 0);
#ifdef MSG_REFERENCE_COUNTING
    if(msg)
    {
        // set non-atomically after we get it from the free list and
        // before we return.
        // no one else can have a reference before that.
        std::atomic_store(&msg->m_referenceCount, 1);
    }
#endif
    if(msg && size > msg->m_bufferSize)
    {
        cout << "ERROR!  Can't allocate " << size << " byte buffer, max size is " << msg->m_bufferSize << endl;
        Free(msg);
        return 0;
    }
    //cout << "MessagePool::Allocate(" << msg << ") success with " <<old_len << "->" << uxQueueMessagesWaiting(m_freeList) << " len queue" << endl;
    return msg;
}
void MessagePool::Free(MessageBuffer* msg)
{
#ifdef MSG_REFERENCE_COUNTING
    if (msg->decrement_refcount() == 1)
#endif
    {
        //int old_len = uxQueueMessagesWaiting(m_freeList);
        BaseType_t ret = xQueueSend(m_freeList, (void*)&msg, 0);
        if(!ret)
        {
            //cout << "MessagePool::Free(" << msg << ") FAIL with " <<old_len << "->" << uxQueueMessagesWaiting(m_freeList) << " len queue" << endl;
            while(uxQueueMessagesWaiting(m_freeList) > 0)
            {
                MessageBuffer* buf = Allocate(1);
                cout << "Had " << buf << " in queue" << endl;
            }
            configASSERT(false);
        }
        else
        {
            //cout << "MessagePool::Free(" << msg << ") success with " <<old_len << "->" << uxQueueMessagesWaiting(m_freeList) << " len queue" << endl;
        }
    }
}
