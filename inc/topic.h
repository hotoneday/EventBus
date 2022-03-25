#ifndef _TOPIC_H_
#define _TOPIC_H_

//here define the topic and topic event which sdk provides
struct myEvent
{
    size_t eventValue;
    std::string message;
    std::string status;
    myEvent() :eventValue(0), message(""), status("") {};
};

#define EVENT_1_FUNCTION void(const std::string&)
#define EVENT_2_FUNCTION void(const int&)
#define EVENT_3_FUNCTION void(const myEvent&)

enum class TOPIC
{
    EVENT_1_TOPIC = 1,
    EVENT_2_TOPIC = 2,
    EVENT_3_TOPIC
};

#endif 
