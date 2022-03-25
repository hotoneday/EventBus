#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "function_traits.h"
#include "topicService.hpp"
#include "topicServicePool.h"
#include "topic.h"

class EventBusInterface
{
public:
    ~EventBusInterface() = default;
    
    //register the Topic with normal funciton/std::function/lambada/function pointer
    template<typename FunctionType>
    void RegisterTopic(const TOPIC& topic, const FunctionType&);

    //register the Topic with member function
    template<typename ClassType, typename Ret, typename... Args, typename ClassInstance>
    void RegisterTopic(const TOPIC& topic, Ret(ClassType::*f)(Args...), ClassInstance& instance);
};


template<typename FunctionType>
void EventBusInterface::RegisterTopic(const TOPIC& topic, const FunctionType& fun)
{
    //you can check the function type with the one you declared
    static_assert(std::is_same<EVENT_1_FUNCTION, function_traits<FunctionType>::function_type>::value ||
                  std::is_same<EVENT_2_FUNCTION, function_traits<FunctionType>::function_type>::value ||
                  std::is_same<EVENT_3_FUNCTION, function_traits<FunctionType>::function_type>::value, "please check the function_event format provided!!!");
    
    function_traits<FunctionType>::stdFunctionType stdFun = static_cast<function_traits<FunctionType>::stdFunctionType>(fun);

    //add the function to pool
    typedef function_traits<FunctionType>::arg_type<0>::type EventType;
    typedef typename std::decay<EventType>::type EventRawType;

    std::shared_ptr<TopicServiceBase> worker =
        std::make_shared <TopicService<EventRawType>>(stdFun);
    
    TopicServicePool::GetInstance().InsertTopicService(topic, worker);
}

//register the member function
template<typename ClassType, typename Ret, typename... Args, typename ClassInstance>
void EventBusInterface::RegisterTopic(const TOPIC& topic, Ret(ClassType::*f)(Args...), ClassInstance& instance)
{
    //firstly check function type
    typedef  Ret(ClassType::*memberFun)(Args...);
    static_assert(std::is_same<EVENT_1_FUNCTION, function_traits<memberFun>::function_type>::value ||
                  std::is_same<EVENT_2_FUNCTION, function_traits<memberFun>::function_type>::value ||
                  std::is_same<EVENT_3_FUNCTION, function_traits<memberFun>::function_type>::value, "please check the function_event format provided!!!");
    
    //get the std function
    std::function<Ret(Args...)> stdFun = [&instance, f](Args... args) {return (instance.*f)(args...); };

    //add the function to pool
    typedef function_traits<memberFun>::arg_type<0>::type EventType;
    typedef typename std::decay<EventType>::type EventRawType;

    std::shared_ptr<TopicServiceBase> worker =
        std::make_shared <TopicService<EventRawType>>(stdFun);

    TopicServicePool::GetInstance().InsertTopicService(topic, worker);
}

#endif
