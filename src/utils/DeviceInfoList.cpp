/*********************************************************************
 *  @file         DeviceInfoList.cpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#include <algorithm>
#include "DeviceInfoList.hpp"
#include "log.hpp"

namespace devicehelper
{
namespace deviceList
{
#undef LOGDEVICELIST

DeviceListErrorCodes DeviceInfoList::AddDevice(const DeviceInfo &deviceInfo)
{
    #ifdef LOGDEVICELIST
    LOG_DEVICELIST << "Add device: " << deviceInfo.serialNumber << "\n";
    #endif
    if (deviceInfo.serialNumber.empty())
    {
        #ifdef LOGDEVICELIST
        LOG_DEVICELIST << "Serial Number empty. Not adding \n";
        #endif
        return DeviceListErrorCodes::UPDATE_DEVICE_CONNECT_FAILED;
    }
    auto helperFun = [&](auto &tempdeviceInfo) { if(tempdeviceInfo.serialNumber == deviceInfo.serialNumber)  return true; else return false; };
    auto listIterator = std::find_if(m_vDeviceinfoList.begin(), m_vDeviceinfoList.end(), helperFun);
    if (listIterator == m_vDeviceinfoList.end())
    {
        m_vDeviceinfoList.emplace_back(deviceInfo);
        return DeviceListErrorCodes::UPDATE_DEVICE_CONNECT_SUCCESSFUL;
    }
    else
    {
        return updateDeviceList(listIterator, deviceInfo);
    }
}


bool DeviceInfoList::checkIfAlreadyExists(DeviceInfo deviceInfo)
{
    bool deviceExists =false;
    #ifdef LOGDEVICELIST
    LOG_DEVICELIST << "Check if device exists \n";
    #endif
    DeviceInfo tdeviceInfo;
    if(DeviceListErrorCodes::FIND_DEVICE_FOUND == findDevicebySerialNumber(deviceInfo.serialNumber, deviceInfo))
    {
        LOG_DEVICELIST << "Devices exists \n";
        deviceExists= true;
    }

    return deviceExists;
}

DeviceListErrorCodes DeviceInfoList::findDevicebySerialNumber(const std::string &serialNumber, DeviceInfo &deviceInfo)
{
    #ifdef LOGDEVICELIST
    LOG_DEVICELIST << "Find device: " << serialNumber << "\n";
    #endif
    if (serialNumber.empty())
    {
        #ifdef LOGDEVICELIST
        LOG_DEVICELIST << "Serial Number empty. \n";
        #endif
        return DeviceListErrorCodes::FIND_DEVICE_NOT_FOUND;
    }
    auto helperFun = [serialNumber](auto &l_deviceInfo) { return l_deviceInfo.serialNumber == serialNumber; };
    auto listIterator = std::find_if(m_vDeviceinfoList.begin(), m_vDeviceinfoList.end(), helperFun);

    if (listIterator == m_vDeviceinfoList.end())
    {
        return DeviceListErrorCodes::FIND_DEVICE_NOT_FOUND;
    }
    deviceInfo = *listIterator;
    return DeviceListErrorCodes::FIND_DEVICE_FOUND;
    }

DeviceListErrorCodes DeviceInfoList::findDevicebyDeviceID(const  int32_t &deviceID, DeviceInfo &deviceInfo)
{
    #ifdef LOGDEVICELIST
    LOG_DEVICELIST << "Find device: " << deviceID << "\n";
    #endif
    if (!deviceID)
    {
        #ifdef LOGDEVICELIST
        LOG_DEVICELIST << "deviceID empty. \n";
        #endif
        return DeviceListErrorCodes::FIND_DEVICE_NOT_FOUND;
    }
    auto helperFun = [&](auto &deviceInfo) { return deviceInfo.deviceID == deviceID;};
    auto listIterator = std::find_if(m_vDeviceinfoList.begin(), m_vDeviceinfoList.end(), helperFun);

    if (listIterator == m_vDeviceinfoList.end())
    {
        return DeviceListErrorCodes::FIND_DEVICE_NOT_FOUND;
    }
    deviceInfo = *listIterator;
    return DeviceListErrorCodes::FIND_DEVICE_FOUND;
}

DeviceListErrorCodes DeviceInfoList::removeDevice(const DeviceInfo &deviceInfo)
{
    #ifdef LOGDEVICELIST
    LOG_DEVICELIST << "Remove device SerialNumber: " << deviceInfo.serialNumber <<"// devicePath: "<<deviceInfo.devicePath<< "\n";
    #endif
    const auto &result = m_vDeviceinfoList.erase(
        std::remove_if(
            m_vDeviceinfoList.begin(), m_vDeviceinfoList.end(),
            [&deviceInfo](const auto &deviceInfoIt)
            {
                if((deviceInfoIt.serialNumber == deviceInfo.serialNumber) ||
                (deviceInfoIt.devicePath == deviceInfo.devicePath))
                {
                    return true;
                }
                else
                    return false;
            }
            ), m_vDeviceinfoList.end());
    if (result != m_vDeviceinfoList.end())
    {
        return DeviceListErrorCodes::REMOVE_DEVICE_FAILED;
    }
    return DeviceListErrorCodes::REMOVE_DEVICE_SUCCESSFUL;
}

DeviceListErrorCodes DeviceInfoList::updateDeviceList(std::vector<DeviceInfo>::iterator  listIterator, const DeviceInfo &deviceInfo)
{
    if (m_vDeviceinfoList.end() == listIterator)
    {
        return DeviceListErrorCodes::UPDATE_DEVICE_CONNECT_FAILED;
    }
    *listIterator = deviceInfo;
    return DeviceListErrorCodes::UPDATE_DEVICE_CONNECT_SUCCESSFUL;
}

void DeviceInfoList::listDevices()
{
    #ifdef LOGDEVICELIST
    LOG_DEVICELIST << "\n ######### DEVICE LIST ######### \n";
    LOG_DEVICELIST << " No of Devices in the List: " << m_vDeviceinfoList.size() << '\n';
    #endif
    for (const auto &deviceInfo : m_vDeviceinfoList)
    {
        LOG_DEVICELIST << (int)deviceInfo.devicestate << "\t"
                            << deviceInfo.serialNumber << "\t" << std::hex
                            << deviceInfo.vendorID << "\t" << std::hex
                            << deviceInfo.productID << "\t" 
                            << deviceInfo.devicePath << "\t"
                            << deviceInfo.busNo << "\t"
                            << deviceInfo.deviceNo << "\t"
                            << '\n';
    }
    #ifdef LOGDEVICELIST
    LOG_DEVICELIST << "\n ######### ########### ######### \n";
    #endif
}

} // namespace deviceList
} // namespace devicehelper