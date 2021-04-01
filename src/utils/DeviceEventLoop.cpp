/*********************************************************************
 *  @file         DeviceEventLoop.cpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#include "DeviceEventLoop.hpp"
#include "log.hpp"
#include <chrono>
#include <atomic>
#include "DeviceInfo.hpp"

namespace devicehelper {

using namespace std::chrono_literals;

#undef LOGDEVICEEVENTLOOP

void DeviceEventLoop::run(std::shared_ptr<MessageQueue> messagequeue, 
std::shared_ptr<deviceList::DeviceInfoList> deviceinfolist,
DevicehelperServer* devicehelperserver)
{
    m_deviceinfolist = deviceinfolist;
    m_devicehelperserver = devicehelperserver;
    while(!m_isQuit)
    {
        #ifdef LOGDEVICEEVENTLOOP
        LOG_DEVICEEVENTLOOP << "Waiting for Device events..\n";
        #endif
        deviceList::DeviceInfo msg;
        std::unique_lock<std::mutex> lock(messagequeue->m_mqmutex);
        messagequeue->m_mqconditionVariable.wait(lock, [messagequeue](){ return !messagequeue->m_msg_queue.empty(); } );
        msg = messagequeue->m_msg_queue.front();        
        messagequeue->m_msg_queue.pop();
        #ifdef LOGDEVICEEVENTLOOP
        LOG_DEVICEEVENTLOOP <<"ProductID:"<< msg.productID<<"\n";
        LOG_DEVICEEVENTLOOP <<"State:"<< (int)msg.devicestate<<"\n";
        LOG_DEVICEEVENTLOOP <<"Serial :"<< msg.serialNumber<<"\n";
        LOG_DEVICEEVENTLOOP <<"path :"<< msg.devicePath<<"\n";
        LOG_DEVICEEVENTLOOP <<"devicenumber :"<< msg.deviceNo<<"\n";
        #endif
        if(m_devicehelperserver->updateDeviceList(msg))
            LOG_DEVICEEVENTLOOP <<"Device("<< msg.mediumName<<") Updated to list and propogated to service\n";
        
    }
}

void DeviceEventLoop::quit()
{
    std::unique_lock<std::mutex> lock(m_deviceEventLoop_mutex);
    m_isQuit = true;
}

}//devicehelper