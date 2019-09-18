#include "message_pool.h"
#include "message_client.h"

//# for memcpy
#include <string.h>
#include <stdio.h>
#include <algorithm> 

#include <iostream>
using namespace std;

int MessageBuffer::increment_refcount()
{
#if defined(MSG_REFERENCE_COUNTING)
    //cout << this << "++" << endl;
    int ret = std::atomic_fetch_add(&m_referenceCount, 1);
    configASSERT(ret > 0);
    return ret;
#else
    return 0;
#endif
}
int MessageBuffer::decrement_refcount()
{
#if defined(MSG_REFERENCE_COUNTING)
    //cout << this << "--" << endl;
    return std::atomic_fetch_sub(&m_referenceCount, 1);
#else
    return 0;
#endif
}

Message::Message()
: m_buf(nullptr),
  m_data(nullptr)
{
}
Message::Message(MessageBuffer* buf)
: m_buf(buf),
  m_data(buf->m_data)
{
    //# can't increment here, we're inheriting a non-zero count from our caller (in MessageQueue::get() or MessagePool::Allocate())
    //buf->increment_refcount();
}
Message::Message(int size)
: m_buf(nullptr),
  m_data(nullptr)
{
    Allocate(size);
}
void Message::InitializeTime()
{
}
bool Message::Allocate(int size)
{
    if(!m_buf)
    {
        //# could add more parameters here if needed, like source, destination?!?
        MessagePool* pool = MessagePool::CurrentPool();
        MessageBuffer* msg_buffer = pool->Allocate(size);
        if(msg_buffer)
        {
            m_buf = msg_buffer;
            m_data = msg_buffer->m_data;
            return true;
        }
        else
        {
            printf("%s line %d, ERROR!  Failed to allocate\n", __FILE__, __LINE__);
        }
    }
    else
    {
        printf("%s line %d, ERROR!  Allocating, but already allocated\n", __FILE__, __LINE__);
    }
    return false;
}
// Copy constructor
Message::Message(const Message &rhs)
: m_buf(nullptr),
  m_data(nullptr)
{
#ifdef MSG_REFERENCE_COUNTING
    m_buf = rhs.m_buf;
    m_data = rhs.m_data;
    rhs.m_buf->increment_refcount();
#else
    Allocate(rhs.GetDataLength());
    // copy the contents
    memcpy(&m_buf->m_hdr, &rhs.m_buf->m_hdr, sizeof(m_buf->m_hdr));
    memcpy(m_data, rhs.m_data, std::min(GetDataLength(), rhs.GetDataLength()));
#endif
}
Message::~Message()
{
    Deallocate();
}
void Message::Deallocate()
{
    if(m_buf != 0)
    {
        // Free back to whatever pool it came from.
        if(m_buf && m_buf->m_owner)
        {
            m_buf->m_owner->Free(m_buf);
        }
        else
        {
            cout << "ERROR!  " << m_buf << endl;
        }
        m_buf = 0;
        m_data = 0;
    }
}
void Message::SetMessageID(MessageIdType id)
{
    m_buf->m_hdr.SetMessageID(id);
}
void Message::SetDataLength(uint16_t len)
{
    m_buf->m_hdr.SetDataLength(len);
}
MessageIdType Message::GetMessageID() const
{
    return m_buf->m_hdr.GetMessageID();
}
int Message::GetDataLength() const
{
    return m_buf->m_hdr.GetDataLength();
}
uint8_t* Message::GetDataPointer() const
{
    return m_buf->m_hdr.m_data;
}
int Message::GetTotalLength() const
{
    return sizeof(m_buf->m_hdr)+GetDataLength();
}
bool Message::Exists()
{
    return m_buf != 0 && m_data != 0;
}
