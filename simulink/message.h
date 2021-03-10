#ifndef MSGTOOLS_MESSAGE_H
#define MSGTOOLS_MESSAGE_H

#include <stdint.h>
#include <assert.h>
#include <vector>
#include <map>

#define UNDEFINED_MSGID (-1)

#define MessageIdType uint64_t

#include <Cpp/headers/NetworkHeader.h>

// This is a C++ implementation of a message.
// It doesn't use Qt or pool allocated messages, but does use the C++ stdlib's vector type to store data.

// Because we don't use a pool allocator, we don't want to define MessageBuffer,
// and Exists() always returns true.
// We still need to define the Message constructor that accepts a MessageBuffer* as
// a parameter, because generated code calls it, if anyone gives a MessageBuffer*
// as a constructor parameter of the generated classes.  This could be eliminated
// if we made a new C++ generated code template file that didn't mention MessageBuffer.
typedef void MessageBuffer;

class Message
{
    public:
        Message()
        {
        }
        Message(uint16_t len)
        {
            m_hdr.SetDataLength(len);
            m_data.reserve(len);
            m_data.insert(m_data.begin(), len, '\0');
        }
        Message(MessageBuffer* buf);
        bool Exists() {return true;}
        uint16_t GetDataLength()
        {
            return m_hdr.GetDataLength();
        }
        void SetDataLength(int len)
        {
            m_hdr.SetDataLength(len);
        }
        void SetMessageID(uint32_t id)
        {
            m_hdr.SetMessageID(id);
        }
        uint32_t GetMessageID()
        {
            return m_hdr.GetMessageID();
        }
        void InitializeTime()
        {
            //# should we set time to simulation time?
            //# where do we get that?
            m_hdr.SetTime(0);
        }
    public:
        NetworkHeader m_hdr;
        std::vector<uint8_t>  m_data;
};

#endif
