#ifndef SIMULINK_MSG_CLIENT_H__
#define SIMULINK_MSG_CLIENT_H__

#include "message_client.h"

class SimulinkMessageClient : public MessageClient
{
    public:
        SimulinkMessageClient();
        static SimulinkMessageClient* Instance();
        void ReadAllMessages();
        Message* RegisterRxBlock(MessageIdType id, unsigned src, unsigned dst);
    private:
        std::map<MessageKey, Message*> m_all_msgs;
};

#endif
