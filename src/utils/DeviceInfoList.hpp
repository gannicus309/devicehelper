/*********************************************************************
 *  @file         DeviceInfoList.hpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#pragma once
#include <vector>
#include <cstdint>
#include <iostream>
#include "DeviceInfo.hpp"
#include <mutex>


namespace devicehelper {
namespace deviceList
{
enum class DeviceListErrorCodes
{
    FIND_DEVICE_FOUND,
    FIND_DEVICE_NOT_FOUND,
    REMOVE_DEVICE_SUCCESSFUL,
    REMOVE_DEVICE_FAILED,
    UPDATE_DEVICE_CONNECT_SUCCESSFUL,
    UPDATE_DEVICE_CONNECT_FAILED,		
    UPDATE_DEVICE_PROFILE_SUCCESSFUL,
    UPDATE_DEVICE_PROFILE_FAILED
};

class DeviceInfoList
{
public:
    DeviceInfoList() = default;
    DeviceInfoList(DeviceInfoList &&) = default;
    DeviceInfoList(const DeviceInfoList &) = default;
    DeviceInfoList &
    operator=(const DeviceInfoList &) = default;
    DeviceInfoList &operator=(DeviceInfoList &&) = default;

    DeviceListErrorCodes AddDevice(const DeviceInfo &deviceInfo);
    bool checkIfAlreadyExists(DeviceInfo deviceInfo);
    DeviceListErrorCodes findDevicebySerialNumber(const std::string &serialNumber, DeviceInfo& deviceinfo) ;
    DeviceListErrorCodes findDevicebyDeviceID(const  int32_t &deviceID, DeviceInfo& deviceinfo) ;
    DeviceListErrorCodes removeDevice(const DeviceInfo &deviceInfo);	
    void listDevices();
    std::mutex m_deviceInfoList_mutex;
    std::vector<DeviceInfo> m_vDeviceinfoList;

private:
    DeviceListErrorCodes updateDeviceList( auto listIterator, const DeviceInfo &deviceInfo);
};

}//deviceList
}//devicehelper