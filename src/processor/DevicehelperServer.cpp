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
#include "AAPConsts.hpp"
namespace devicehelper
{

void DevicehelperServer::deviceDetection_worker()
{
    LOG_GENERAL << "Create worker thread for monitoring\n";
    m_usbDeviceDetector->udev_initialize();
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
    std::shared_ptr<detector::USBDetector> usbdetector,
    std::shared_ptr<MessageQueue> messagequeue,
    std::shared_ptr<DeviceEventLoop> deviceEventLoop) :
    m_usbDeviceDetector{usbdetector},
    m_messageQueue{messagequeue},
    m_deviceEventLooper{deviceEventLoop}
{
    m_deviceInfoList = std::make_shared<deviceList::DeviceInfoList>();
    m_libUsbWrapper = std::make_unique<deviceList::LibUsbWrapper>();
}

DevicehelperServer::~DevicehelperServer()
{
    if (m_devicemonitorThread.joinable())
        m_devicemonitorThread.join();

    m_deviceEventLooper->quit();
    if(m_deviceEventLoopThread.joinable())
        m_deviceEventLoopThread.join();
}

void DevicehelperServer::startUSBMonitor()
{
    m_devicemonitorThread = std::thread(&DevicehelperServer::deviceDetection_worker, this);
    m_deviceEventLoopThread = std::thread(&DevicehelperServer::deviceEventLoop_worker, this);
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
    bool deviceListChanged = false;
    {    
        switch(deviceinfo.devicestate)
        {
            case deviceList::DeviceState::DETECTION:
                //First update the device info using Libusb wrappers
                if(deviceList::FillDeviceDetailsErrorCodes::DEVICE_OPEN_FAILED == m_libUsbWrapper->fillDeviceDetails(deviceinfo))
                {
                    LOG_GENERAL << "[ERR] Fill devices from libusb failed. Exit\n";
                    return false;
                }
                deviceinfo.deviceID = convertSerialNumberToDeviceID(deviceinfo.serialNumber);
                LOG_GENERAL << "Trying to add device to deviceinfolist\n";

                /*MOCKING ALL DEVICES TO MTP/PTP TODO*/
                deviceinfo.deviceType = deviceList::DeviceType::MTP_DEVICE;
                deviceinfo.device = deviceList::Device::USB1;
                deviceinfo.activeProfile = deviceList::ProfileType::PTP;
                if(!(deviceinfo.productID >= AOAP_PRODUCT_MIN && deviceinfo.productID <= AOAP_PRODUCT_MAX)) // Check if google android auto
                {
                    deviceinfo.activeProfile = deviceList::ProfileType::AOAP;
                    LOG_GENERAL << "Update to AOAP\n";
                }
                deviceinfo.profileList ={deviceList::ProfileType::PTP,deviceList::ProfileType::AOAP};
                deviceinfo.devicestate = deviceList::DeviceState::READY;
                ///////////////////////////////////////
                {
                    std::unique_lock<std::mutex> lock(m_deviceInfoList->m_deviceInfoList_mutex);
                    if(deviceList::DeviceListErrorCodes::UPDATE_DEVICE_CONNECT_SUCCESSFUL != m_deviceInfoList->AddDevice(deviceinfo))
                    {
                        LOG_GENERAL << "[ERR] Device cannot be added\n";
                        deviceListChanged = false;
                        break;
                    }
                }
                LOG_GENERAL << "DEVICE ADDED//[DEVICE]>>>>>>>>>>>>>>>>>>>>>>>>>>>[LIST]\n";
                deviceListChanged = true;
                break;

            case deviceList::DeviceState::EMPTY:
                LOG_GENERAL << "Trying to remove device from deviceinfolist\n";
                {
                    std::unique_lock<std::mutex> lock(m_deviceInfoList->m_deviceInfoList_mutex);
                    if(deviceList::DeviceListErrorCodes::REMOVE_DEVICE_SUCCESSFUL != m_deviceInfoList->removeDevice(deviceinfo))
                    {
                        LOG_GENERAL << "[ERR] Device is not in list or cannot remove\n";
                        deviceListChanged = false;
                        break;
                    }
                }
                LOG_GENERAL << "DEVICE REMOVED//[DEVICE]<<<<<<<<<<<<<<<<<<<<<<<<<<<[LIST]\n";
                deviceListChanged = true;
                break;

            case deviceList::DeviceState::READY:
            case deviceList::DeviceState::PROFILE_CHANGE:
                LOG_GENERAL << "Update device to new state: "<<(int)deviceinfo.devicestate <<",profile : "<< (int)deviceinfo.activeProfile<<"\n";
                {
                    std::unique_lock<std::mutex> lock(m_deviceInfoList->m_deviceInfoList_mutex);
                    if(deviceList::DeviceListErrorCodes::UPDATE_DEVICE_CONNECT_SUCCESSFUL != m_deviceInfoList->AddDevice(deviceinfo))
                    {
                        LOG_GENERAL << "[ERR] Device cannot be updated\n";
                        deviceListChanged = false;
                        break;
                    }
                }
                LOG_GENERAL << "DEVICE UPDATED//[DEVICE]>>>>>>>>>>>>>>>>>>>>>>>>>>>[LIST]\n";
                deviceListChanged = true;
                break;

            default:
                //TODO Add event handling for update of devices
                LOG_GENERAL << "Default\n";
                break;
        }

        m_deviceInfoList->listDevices();
    }

    if(deviceListChanged)
        DevicelistChanged();

    return deviceListChanged;
}

void DevicehelperServer::DevicelistChanged()
{
    std::vector<deviceList::DeviceInfo> l_deviceinfolist;
    {
        std::unique_lock<std::mutex> lock(m_deviceInfoList->m_deviceInfoList_mutex);
        LOG_GENERAL << "Get DeviceInfo list\n";
        m_deviceInfoList->listDevices();    
        l_deviceinfolist = m_deviceInfoList->m_vDeviceinfoList;
    }

    //Send changdeviceprofile.. Has to be sent from client
    //Debug: Test only
    //changeDeviceProfile(convertSerialNumberToDeviceID("22b5bea4"),deviceList::ProfileType::AOAP);
}

void DevicehelperServer::changeDeviceProfile(const int32_t deviceID,const deviceList::ProfileType profile)
{
    deviceList::DeviceInfo device;
    m_deviceInfoList->findDevicebyDeviceID(deviceID,device);
    LOG_GENERAL << "Device: vendor:"<< device.vendorID <<"\n";
    
    // Profile Change ongoing..
    device.devicestate = deviceList::DeviceState::PROFILE_CHANGE;
    device.activeProfile = profile;
    updateDeviceList(device); // Update device state

    LOG_GENERAL << "Profile will be changed to "<<std::hex<< (int)profile <<"\n";

    auto handle = std::async(std::launch::async,
                            &deviceList::LibUsbWrapper::changeDeviceProfile,m_libUsbWrapper.get(),device.vendorID,device.productID);

    //Wait until changes profile or Timeout of 1 second
    handle.wait_until(std::chrono::system_clock::now() + std::chrono::seconds(1));
    if(deviceList::ChangeProfileErrorCodes::CHANGE_PROFILE_SUCCESS == handle.get())
    {
        LOG_GENERAL << "################### Profile changed... PHONE NOW IN AOAP MODE #####################\n";
        device.devicestate = deviceList::DeviceState::READY;
        device.activeProfile = deviceList::ProfileType::AOAP;
        updateDeviceList(device); // Update device state
    }
    else
    {
        LOG_GENERAL << "Error in Profile changed\n";
    }
}

} // end namespace devicehelper