/*********************************************************************
 *  @file         DevicehelperServer.cpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#include <exception>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <functional>
#include <chrono>
#include <future>
#include "log.hpp"
#include "USBDetector.hpp"
#include "DevicehelperServer.hpp"

namespace devicehelper
{

void DevicehelperServer::deviceDetection_worker()
{
    LOG_GENERAL << "Create worker thread for monitoring\n";
    m_usbDeviceDetector->udev_initialize(m_messageQueue);
}

void DevicehelperServer::deviceEventLoop_worker()
{
    LOG_GENERAL << "Create Event loop thread for monitoring\n";
    m_deviceEventLooper->run(m_messageQueue,m_deviceInfoList,this);
}


void DevicehelperServer::libUsb_changeProfile(int32_t vendorID, int32_t productID)
{
    LOG_GENERAL << "Change LibUSB profile\n";
}

DevicehelperServer::DevicehelperServer(
    std::shared_ptr<detector::USBDetector> usbdetector) :
    m_usbDeviceDetector{usbdetector}
{
    m_deviceInfoList = std::make_shared<deviceList::DeviceInfoList>();
    m_libUsbWrapper = std::make_unique<deviceList::LibUsbWrapper>();
}

DevicehelperServer::~DevicehelperServer()
{
    if (m_devicemonitorThread.joinable())
        m_devicemonitorThread.join();
}

void DevicehelperServer::startUSBMonitor()
{
    m_devicemonitorThread = std::thread(&DevicehelperServer::deviceDetection_worker, this);
}

std::vector<deviceList::DeviceInfo> DevicehelperServer::getDeviceInfoList()
{
    std::unique_lock<std::mutex> lock(m_deviceInfoList->m_deviceInfoList_mutex);
    LOG_GENERAL << "Get DeviceInfo list\n";
    m_deviceInfoList->listDevices();
    return m_deviceInfoList->m_vDeviceinfoList;
}

bool DevicehelperServer::updateDeviceList(deviceList::DeviceInfo deviceinfo)
{
}

void DevicehelperServer::DevicelistChanged()
{
}

void DevicehelperServer::changeDeviceProfile(const int32_t deviceID,const deviceList::ProfileType profile)
{
}

} // end namespace devicehelper