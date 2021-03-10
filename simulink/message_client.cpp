#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "message_client.h"
#include "Cpp/Network/Connect.h"
#include "Cpp/Network/MaskedSubscription.h"

#define OPEN_RETRY_COUNT 25

MessageClient::MessageClient(int port)
: m_sock(0),
  m_port(port),
  m_connectRetries(OPEN_RETRY_COUNT)
{
    printf("Constructing MessageClient\n");
}

bool MessageClient::OpenSocket()
{
    if(m_sock != 0)
    {
        return true;
    }
    if(++m_connectRetries < OPEN_RETRY_COUNT)
    {
        return false;
    }
    m_connectRetries = 0;
    m_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(m_sock < 0) 
    {
        printf("\n Socket creation error \n");
        m_sock = 0;
        return false;
    }
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(m_port);
    
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\ninet_pton Failed \n");
        close(m_sock);
        m_sock = 0;
        return false;
    }
    
    if (connect(m_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        close(m_sock);
        m_sock = 0;
        return false;
    }
    printf("\n MessageClient::OpenSocket(), m_sock is open!! \n");
    {
        ConnectMessage cm;
        strcpy((char*)cm.Name(), "Simulink");
        SendMessage(cm);
    }
    {
        MaskedSubscriptionMessage subscribeMsg;
        SendMessage(subscribeMsg);
    }
    return true;
}

bool MessageClient::SendMessage(Message& msg)
{
    if(OpenSocket())
    {
        send(m_sock, msg.m_hdr.m_data, sizeof(msg.m_hdr.m_data), 0);
        send(m_sock, &msg.m_data[0], msg.GetDataLength(), 0);
        return true;
    }
    return false;
}

bool MessageClient::ReadHeader(NetworkHeader& hdr)
{
    int ret = recv(m_sock, hdr.m_data, sizeof(hdr.m_data), MSG_DONTWAIT);
    if(ret < 0)
    {
        if(errno != EAGAIN)
        {
            printf("%d = recv(%d), errno = %d\n", ret, (int)sizeof(hdr.m_data), errno);
        }
        return false;
    }
    else if(ret == 0)
    {
        // recv returns zero when the socket is closed
        close(m_sock);
        m_sock = 0;
        return false;
    }
    else if(ret != sizeof(hdr.m_data))
    {
        printf("MessageClient::ReadHeader(), recv returned %d\n", ret);
        return false;
    }
    return true;
}

bool MessageClient::ReadBody(NetworkHeader& hdr, Message& msg)
{
    int len = hdr.GetDataLength();
    if(len == 0)
    {
        return true;
    }
    //printf("    Got hdr for %d byte msg\n", m_buf->m_hdr.GetDataLength());
    // adjust size of buffer if necessary
    if(len > (int)msg.m_data.capacity())
    {
        msg.m_data.reserve(len);
    }
    int ret = recv(m_sock, &msg.m_data[0], len, 0);
    if(ret < 0)
    {
        printf("ERROR receiving %d bytes on socket\n", len);
        return false;
    }
    else if(ret == 0)
    {
        // recv returns zero when the socket is closed
        close(m_sock);
        m_sock = 0;
        return false;
    }
    else if(ret != len)
    {
        printf("MessageClient::ReadBody(), recv returned %d\n", ret);
        return false;
    }
    return true;
}

bool MessageClient::ReadMessage(Message& msg)
{
    if(ReadHeader(msg.m_hdr))
    {
        return ReadBody(msg.m_hdr, msg);
    }
    return false;
}
