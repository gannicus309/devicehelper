/*********************************************************************
 *  @file         DeviceEventLoop.hpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#pragma once

#include <mutex>
#include <condition_variable>
#include <chrono>
#include "Messagequeue.hpp"
#include "DeviceInfoList.hpp"
#include "processor/DevicehelperServer.hpp"
/**
 * @brief Event loop to wait for notifications from the device detection worker thread
 * @details Event loop to wait for notifications from the device detection worker thread
 */
namespace devicehelper {
namespace deviceList{
class DeviceInfoList;
}
class DevicehelperServer;
class DeviceEventLoop
{
public:

    void run(std::shared_ptr<MessageQueue> messagequeue,
    std::shared_ptr<deviceList::DeviceInfoList> deviceinfolist, 
    DevicehelperServer* devicemanagerserver);
    void quit();
    void updateDeviceList(const deviceList::DeviceInfo deviceinfo);

    std::mutex m_deviceEventLoop_mutex;
    std::condition_variable m_deviceEventLoop_cv;
private:
    bool m_isQuit = false;
    std::shared_ptr<deviceList::DeviceInfoList> m_deviceinfolist;
    DevicehelperServer* m_devicehelperserver;
};

}//devicehelper
