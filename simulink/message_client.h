#ifndef MSG_CLIENT_H
#define MSG_CLIENT_H

#include <cstring>
#include "message.h"
#include "message_key.h"

// This is a C++ implementation of reading a message from a TCP socket.
// It doesn't use Qt, but does use the C++ stdlib's vector type.
class MessageClient
{
    public:
        MessageClient(int port=5678);
        bool OpenSocket();
        bool SendMessage(Message& msg);
        bool ReadMessage(Message& msg);
    protected:
        bool ReadHeader(NetworkHeader& hdr);
        bool ReadBody(NetworkHeader& hdr, Message& msg);
    private:
        int m_sock;
        int m_port;
        int m_connectRetries;
};

#endif
