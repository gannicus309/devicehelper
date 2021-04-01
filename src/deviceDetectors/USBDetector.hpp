/*********************************************************************
 *  @file         USBDetector.hpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#pragma once

#include <libudev.h>
#include <memory>
#include "DeviceInfo.hpp"
#include "MessageQueue.hpp"

namespace devicehelper
{
namespace detector
{

class USBDetector
{
public:
    USBDetector() = default;

    USBDetector(USBDetector &&) = delete;
    USBDetector(const USBDetector &) = default;
    USBDetector &
    operator=(const USBDetector &) = delete;
    USBDetector &operator=(USBDetector &&) = delete;

    ~USBDetector() = default;

    int udev_initialize(std::shared_ptr<MessageQueue> messagequeue);
    void monitor_devices(struct udev *udev);
    void enumerate_devices(struct udev *udev);
    void process_device(struct udev_device *dev);
    void print_device(const deviceList::DeviceInfo &deviceInfo);

private:
    std::shared_ptr<MessageQueue> m_messageQueue;
};

} // namespace detector
} // namespace devicehelper
