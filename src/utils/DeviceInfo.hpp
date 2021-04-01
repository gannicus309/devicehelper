/*********************************************************************
 *  @file         DeviceInfo.hpp
 *  @author       Ganesh Rengasamy
 *********************************************************************/

#pragma once
#include <string>
#include "DeviceEnums.hpp"
namespace devicehelper {
namespace deviceList {

struct DeviceInfo
{
    Device device;
    DeviceType deviceType;
    DeviceState devicestate;
    ProfileType activeProfile;
    USBDeviceClassCode usbDeviceClass;
    int32_t deviceID;
    int32_t vendorID;
    int32_t productID;
    int32_t busNo;
    int32_t deviceNo;
    uint8_t usbDeviceSubClass;
    std::string devicePath;
    std::string mediumName;
    std::string descriptorInformation;
    std::string serialNumber;
};

}
}
