#include <iostream>
#include <functional>
#include <type_traits>
#include <tuple>
#include "topicServicePool.h"
#include "eventBusInterface.h"


void onStringEventNotify(const std::string& message)
{
    std::cout << "call the back fun" << std::endl;
}

typedef void(*myEventPtr)(const myEvent& messge);

void onMyEventNotify(const myEvent& message)
{
    std::cout << "my Event message comes: value: " << message.eventValue << std::endl;
}

class Foo
{
public:
    Foo() = default;
    ~Foo() = default;
    void onMemberCallback(const int&)
    {
        std::cout << "this is the member callback message" << std::endl;
    }
};


int main()
{
    EventBusInterface* eventBus = new EventBusInterface();
    
    //normal function
    eventBus->RegisterTopic(TOPIC::EVENT_1_TOPIC, onStringEventNotify);

    //member function
    Foo aFoo;
    eventBus->RegisterTopic(TOPIC::EVENT_2_TOPIC, &Foo::onMemberCallback, aFoo);
    
    //function ptr
    myEventPtr eventPtr = onMyEventNotify;
    eventBus->RegisterTopic(TOPIC::EVENT_3_TOPIC, eventPtr);

    //generate the message
    std::string message = "this is the callback";
    TopicServicePool::GetInstance().AddEvent(TOPIC::EVENT_1_TOPIC, message);

    //generate the message for member function
    int number = 0;
    TopicServicePool::GetInstance().AddEvent(TOPIC::EVENT_2_TOPIC, number);

    //generate myEvent message
    myEvent aEvent;
    aEvent.eventValue = 100;
    TopicServicePool::GetInstance().AddEvent(TOPIC::EVENT_3_TOPIC, aEvent);

    getchar();
    return 0;
}
