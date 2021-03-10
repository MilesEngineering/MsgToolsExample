#include "simulink_message_interface.h"
#include "simulink_message_client.h"

#include <stdio.h>

extern "C" bool allocate_msg(uint32_t id, int size, int src, int dst, void** msgbuf, uint8_t** data)
{
    Message* msg = new Message(size);
    if(msg)
    {
        msg->SetMessageID(id);
        msg->m_hdr.SetSource(src);
        msg->m_hdr.SetDestination(dst);
        *msgbuf = (void*)msg;
        *data = &msg->m_data[0];
        //printf("allocate_msg ret true\n");
        return true;
    }
    *data = NULL;
    printf("allocate_msg ret FALSE\n");
    return false;
}

extern "C" void send_msg(void** msgbuf)
{
    Message* msg = (Message*)*msgbuf;
    SimulinkMessageClient::Instance()->SendMessage(*msg);

    // clear the user's pointer
    *msgbuf = NULL;
    delete msg;
}

extern "C" uint8_t* message_rx_data(uint32_t message_id, int src, int dst)
{
    //# poll for new messages.  Should we do this here, every time any block wants data?
    //# or somehow make it happen once per simulink model execution?
    SimulinkMessageClient::Instance()->ReadAllMessages();
    Message* msg = SimulinkMessageClient::Instance()->RegisterRxBlock(message_id, src, dst);
    if(msg)
    {
        //printf("message_rx_data %d:%d->%d valid\n", message_id, src, dst);
        return &msg->m_data[0];
    }
    //printf("message_rx_data %d:%d->%d INVALID\n", message_id, src, dst);
    return NULL;
}
