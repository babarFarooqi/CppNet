#ifndef NET_EVENT_WIN_IOCP_ACTION
#define NET_EVENT_WIN_IOCP_ACTION

#include "../action_interface.h"

namespace cppnet {

// epoll event interface
class IOCPEventActions:
    public EventActions {
public:
    IOCPEventActions();
    virtual ~IOCPEventActions();

    virtual bool Init(uint32_t thread_num = 0);
    virtual bool Dealloc();
    // net io event
    virtual bool AddSendEvent(std::shared_ptr<Event>& event);
    virtual bool AddRecvEvent(std::shared_ptr<Event>& event);
    virtual bool AddAcceptEvent(std::shared_ptr<Event>& event);

    virtual bool AddConnection(std::shared_ptr<Event>& event, Address& address);
    virtual bool AddDisconnection(std::shared_ptr<Event>& event);

    virtual bool DelEvent(std::shared_ptr<Event>& event);
    // io thread process
    virtual void ProcessEvent(int32_t wait_ms);
    // weak up net io thread
    virtual void Wakeup();
private:
    bool AddToIOCP(uint64_t sock);

protected:
    void*     _iocp_handler;
};

}

#endif