#ifndef MESSAGE_CLIENT_H
#define MESSAGE_CLIENT_H

#include "message.h"
#include "message_queue.h"

#define DEFAULT_STACK_SIZE 256
#define DEFAULT_PRIORITY     0

/*
    This class handles most of the commonality associated with any application.
    Descendants of this class need to:
        * implement HandleReceivedMessage(), to do what they wish with received msgs
        * call SendMessage() when they want to transmit
*/
class MessageClient
{
    public:
        MessageClient(const char* name, MessagePool* pool, int period=0, int priority=DEFAULT_PRIORITY, int stacksize=DEFAULT_STACK_SIZE);
        void RunLoop();
		void SendMessage(Message& msg);
        static MessageClient* CurrentClient();
        MessagePool* GetMessagePool();
        void DeliverMessage(Message& msg);
	protected:
        virtual void HandleReceivedMessage(Message& msg/*, MsgInfo* msgInfo*/) = 0;
		virtual void PeriodicTask();
	private:
        TaskHandle_t m_taskHandle;
		// how often the periodic function should be invoked - zero for never
		int m_period;
        // last time we called our periodic function
		TickType_t m_lastIdleTime;
		// count of errors encountered when trying to transmit
		int m_txErrors;
		int m_allocErrors;
        MessagePool* m_msgPool;
        MessageQueue m_rxMsgs;
};

#endif
