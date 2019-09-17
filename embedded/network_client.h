#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include <sys/socket.h> 
#include <arpa/inet.h>

class NetworkClient : public MessageClient
{
    public:
        NetworkClient(MessagePool& pool, int port=5678)
        : MessageClient("Network", &pool, 0, 0, 100),
          m_sock(0),
          m_port(port)
        {
            MessageBus::SubscribeAll(this);
            
            OpenSocket();
        }
        void OpenSocket()
        {
            m_sock = socket(AF_INET, SOCK_STREAM, 0);
            if(m_sock < 0) 
            {
                printf("\n Socket creation error \n");
                return;
            }
            struct sockaddr_in serv_addr;
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(m_port);
            
            if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
            {
                return;
            }
            
            if (connect(m_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
            {
                printf("\nConnection Failed \n");
                return;
            }
        }
        void HandleReceivedMessage(Message& msg)
        {
            // send to network!
            send(m_sock, msg.GetDataPointer(), msg.GetTotalLength(), 0);
        }
        void PeriodicTask()
        {
            // read from network!
        }
    private:
        int m_sock;
        int m_port;
};

#endif
