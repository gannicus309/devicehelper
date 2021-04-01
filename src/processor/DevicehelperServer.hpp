/*********************************************************************
 *  @file         devicehelperserver.hpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#pragma once

#include <mutex>
#include <thread>
#include "libusb_wrapper.hpp"
#include "IDevicehelperServer.hpp"
#include "USBDetector.hpp"
#include "DeviceInfoList.hpp"
#include "DeviceEventLoop.hpp"
#include "MessageQueue.hpp"
namespace devicehelper
{
class DeviceEventLoop;
class DevicehelperServer : public IDevicehelperServer // NOLINT
{
public:
    explicit DevicehelperServer(std::shared_ptr<detector::USBDetector> usbdetector,
        std::shared_ptr<MessageQueue> messagequeue,
        std::shared_ptr<DeviceEventLoop> deviceEventLoop);

    DevicehelperServer(DevicehelperServer &&) = delete;
    DevicehelperServer(const DevicehelperServer &) = delete;
    DevicehelperServer &
    operator=(const DevicehelperServer &) = delete;
    DevicehelperServer &operator=(DevicehelperServer &&) = delete;

    ~DevicehelperServer();

    void deviceDetection_worker();
    void deviceEventLoop_worker();
    void libUsb_changeProfile(int32_t vendorID, int32_t productID);
    void startUSBMonitor();
    std::vector<deviceList::DeviceInfo> getDeviceInfoList() override;
    void DevicelistChanged() override;
    void changeDeviceProfile(const int32_t deviceID,const deviceList::ProfileType profile) override;
    bool updateDeviceList(deviceList::DeviceInfo deviceinfo);
    std::shared_ptr<deviceList::DeviceInfoList> m_deviceInfoList;

private:
    std::shared_ptr<detector::USBDetector> m_usbDeviceDetector;
    std::unique_ptr<deviceList::LibUsbWrapper> m_libUsbWrapper;
    std::thread m_devicemonitorThread;
    std::thread m_deviceEventLoopThread;
    std::shared_ptr<MessageQueue> m_messageQueue;
    std::shared_ptr<DeviceEventLoop> m_deviceEventLooper;

};

} // namespace devicehelper
