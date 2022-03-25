#ifndef _TOPIC_SERVICE_H
#define _TOPIC_SERVICE_H
#include <queue>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <atomic>
#include <thread>

#include "topic.h"


class TopicServiceBase
{
public:
    virtual ~TopicServiceBase() = default;
};

// the default max event size for queue is 1024
// user can set it when construct the service
template <typename Event, size_t MaxEventSize = 1024>
class TopicService : public TopicServiceBase
{
public:
    typedef std::function<void(const Event& eventMessage)> cb_function;

    TopicService(const cb_function& cb)
        : m_cb(cb)
    {
        m_bStop = false;
        Start();
    }
    TopicService() = default;
    ~TopicService();
    void Start();
    void Stop();
    void AddEvent(const Event& event);

private:
    void                         WorkingProc();  // working proc for thread
    std::queue<Event>            m_eventQ;
    std::unique_ptr<std::thread> m_worker;

    std::mutex              m_mux;
    std::condition_variable m_cond;
    cb_function             m_cb;
    std::atomic_bool        m_bStop;
    constexpr static size_t m_maxEventSize = MaxEventSize;
};

template <typename Event, size_t MaxEventSize>
void TopicService<Event, MaxEventSize>::Start()
{
    if (!m_worker)
    {
        m_worker = std::unique_ptr<std::thread>(
            new std::thread(std::bind(&TopicService<Event>::WorkingProc, this)));
    }
}

template <typename Event, size_t MaxEventSize>
void TopicService<Event, MaxEventSize>::WorkingProc()
{
    while (true)
    {
        std::unique_lock<std::mutex> lck(m_mux);
        m_cond.wait(lck, [this] { return m_bStop || !m_eventQ.empty(); });
        if (m_bStop)
        {
            // the worker has stopped return
            return;
        }

        // access the head event
        Event tmp = m_eventQ.front();
        m_eventQ.pop();
        lck.unlock();

        m_cb(tmp);
    }
}

template <typename Event, size_t MaxEventSize>
void TopicService<Event, MaxEventSize>::AddEvent(const Event& event)
{
    std::unique_lock<std::mutex> lck(m_mux);
    if (m_eventQ.size() < m_maxEventSize)
    {
        m_eventQ.push(event);
        m_cond.notify_one();
    }
}

template <typename Event, size_t MaxEventSize>
void TopicService<Event, MaxEventSize>::Stop()
{
    m_bStop = true;
}

template <typename Event, size_t MaxEventSize>
TopicService<Event, MaxEventSize>::~TopicService()
{
    Stop();
    // notify to stop workingProc
    {
        std::unique_lock<std::mutex> lck(m_mux);
        m_cond.notify_one();
    }

    if (m_worker)
    {
        m_worker->join();
        m_worker.reset();
    }
    std::queue<Event> emptyQ;
    m_eventQ.swap(emptyQ);
}
#endif
