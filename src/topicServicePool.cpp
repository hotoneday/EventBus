#include "topicServicePool.h"
#include "iostream"


TopicServicePool* TopicServicePool::g_pool = nullptr;
TopicServicePool& TopicServicePool::GetInstance()
{
    if (!g_pool)
    {
        g_pool = new TopicServicePool();
    }

    return *g_pool;
}

void TopicServicePool::InsertTopicService(const TOPIC& topicType, const topicHandlerBasePtr& workerPtr)
{
    m_map.insert(std::make_pair(topicType, workerPtr));
}

topicHandlerBasePtr TopicServicePool::GetTopicService(const TOPIC& topicType)
{
    auto it = m_map.find(topicType);
    if (it != m_map.end())
    {
        return it->second;
    }

    return nullptr;
}

TopicServicePool::~TopicServicePool()
{
    std::cout<< "Erase worker from event handler pool";
    auto it = m_map.begin();

    for (; it != m_map.end();)
    {
        it = m_map.erase(it);
    }
}
