#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "simulink_message_client.h"
#include "Cpp/Network/Connect.h"
#include "Cpp/Network/MaskedSubscription.h"

SimulinkMessageClient::SimulinkMessageClient()
: MessageClient()
{
}

SimulinkMessageClient* SimulinkMessageClient::Instance()
{
    static SimulinkMessageClient smc;
    return &smc;
}

void SimulinkMessageClient::ReadAllMessages()
{
    if(!OpenSocket())
    {
        printf("SimulinkMessageClient::ReadAllMessages(), not reading from socket, can't open it!\n");
        return;
    }
    // read all the messages from the network until there's none left to read.
    while(1)
    {
        // read the header, so we know what kind of message we're getting
        NetworkHeader hdr;
        if(!ReadHeader(hdr))
        {
            //printf("SimulinkMessageClient::ReadAllMessages(), nothing to read\n");
            return;
        }
        // look up if simulink wants the message, and if so, read it.
        MessageKey key(hdr);
        auto it{ m_all_msgs.find(key) };
        if(it != std::end(m_all_msgs))
        {
            //printf("SimulinkMessageClient::ReadAllMessages(), got header %ld:%d->%d\n", hdr.GetMessageID(), hdr.GetSource(), hdr.GetDestination());
            auto msg{ it->second };
            if(!msg)
            {
                //printf("SimulinkMessageClient::ReadAllMessages(), initializing Message\n");
                msg = new Message(hdr.GetDataLength());
                m_all_msgs[key] = msg;
            }
            
            //# copy the header, which shouldn't do anything except update timestamp and possibly length
            msg->m_hdr = hdr;
            //printf("SimulinkMessageClient::ReadAllMessages(), reading body\n");
            ReadBody(hdr, *msg);
        }
        else
        {
            // throw away the data from the socket, so we don't lose sync on headers/bodies in the data stream.
            Message msg(hdr.GetDataLength());
            ReadBody(hdr, msg);
        }
    }
}

Message* SimulinkMessageClient::RegisterRxBlock(MessageIdType id, unsigned src, unsigned dst)
{
    MessageKey key(id, src, dst);
    if(m_all_msgs.count(key) == 0)
    {
        // set a null pointer to indicate simulink wants the message.
        m_all_msgs[key] = NULL;
    }
    return m_all_msgs[key];
}
