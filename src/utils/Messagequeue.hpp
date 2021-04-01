/*********************************************************************
 *  @file         Messagequeue.hpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#pragma once

#include <iostream>
#include <queue>
#include <condition_variable>
#include "DeviceInfo.hpp"

/**
 * @brief Message queue for Inter thread communication
 * @details Message queue for Inter thread communication
 */
namespace devicehelper {
class MessageQueue
{
public:
    MessageQueue() = default;
    MessageQueue(const MessageQueue &) = delete;
    MessageQueue(const MessageQueue &&) = delete;

    MessageQueue &operator=(MessageQueue &&) = delete;
    MessageQueue &operator=(const MessageQueue &) = delete;
    ~MessageQueue() = default;

    std::queue<deviceList::DeviceInfo> m_msg_queue;
    std::mutex m_mqmutex;
    std::condition_variable m_mqconditionVariable;
};
} // namespace devicehelper
