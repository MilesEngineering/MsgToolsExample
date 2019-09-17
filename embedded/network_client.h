#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

class NetworkClient : public MessageClient
{
    public:
        NetworkClient(MessagePool& pool)
        : MessageClient("Network", &pool, 0, 0, 100)
        {
            MessageBus::SubscribeAll(this);
        }
        void HandleReceivedMessage(Message& msg)
        {
            // send to network!
            cout << "  NetworkClient got " << msg.GetMessageID() << " at time " << xTaskGetTickCount() << endl;
        }
        void PeriodicTask()
        {
            // read from network!
        }
};

#endif
