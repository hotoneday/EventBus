#ifndef _TOPIC_PUBLISHER_POOL_H_
#define _TOPIC_PUBLISHER_POOL_H_
#include "topicService.hpp"

typedef typename std::shared_ptr<TopicServiceBase> topicHandlerBasePtr;

struct TopicHasher
{
    size_t operator()(const TOPIC& aType) const
    {
        return std::hash<size_t>()(size_t(aType));
    }
};

// this class to manage topic publisher, it is a single instance
class TopicServicePool
{
public:
    typedef typename std::unordered_map<TOPIC, topicHandlerBasePtr, TopicHasher> poolHashMap;
    ~TopicServicePool();

    TopicServicePool(const TopicServicePool& copyPool) = delete;
    TopicServicePool(TopicServicePool&& rvaluePool) = delete;
    TopicServicePool& operator=(const TopicServicePool& copyPool) = delete;

    static TopicServicePool& GetInstance();
    void   InsertTopicService(const TOPIC&, const topicHandlerBasePtr& workerPtr);
    
    template<typename Event>
    void AddEvent(const TOPIC& aTopic, const Event& eventMessage);

private:
    TopicServicePool() = default;
    poolHashMap m_map;
    static TopicServicePool* g_pool;

    topicHandlerBasePtr GetTopicService(const TOPIC& topicType);
};

template<typename Event>
void TopicServicePool::AddEvent(const TOPIC& aTopic, const Event& eventMessage)
{
    auto it = GetTopicService(aTopic);
    if (it)
    {
        auto workerPtr = std::dynamic_pointer_cast<TopicService<Event>>(it);
        workerPtr->AddEvent(eventMessage);
    }
}
#endif
